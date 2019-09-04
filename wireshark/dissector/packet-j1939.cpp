extern "C" {

#include "config.h"

#include <epan/packet.h>
#include <epan/address_types.h>
#include <wsutil/pint.h>



//For BAM reassemble
#include <epan/reassemble.h>

void proto_register_j1939(void);
void proto_reg_handoff_j1939(void);

}

#include <iostream>
#include <J1939Factory.h>
#include <J1939Frame.h>
#include <J1939DataBase.h>
#include <GenericFrame.h>
#include <SPN/SPNNumeric.h>
#include <SPN/SPNStatus.h>
#include <SPN/SPNString.h>
#include <Transport/BAM/BamReassembler.h>
#include <FMS/TellTale/FMS1Frame.h>
#include <Diagnosis/Frames/DM1.h>


#ifndef DATABASE_PATH
#define DATABASE_PATH		"/etc/j1939/frames.json"
#endif


using namespace J1939;

int dissect_J1939(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree, void* data);
void dissect_generic_frame(tvbuff_t *tvb, proto_tree *j1939_tree, proto_item *ti, GenericFrame *genFrame);
void dissect_fms1_frame(tvbuff_t *tvb, proto_tree *j1939_tree, proto_item *ti, const FMS1Frame *fms1Frame);
void dissect_dm1_frame(tvbuff_t *tvb, proto_tree *j1939_tree, proto_item *ti, const DM1 *dm1Frame);


static int hf_j1939_frame = -1;
static int hf_j1939_spn = -1;
static int hf_j1939_dtc = -1;
static int hf_j1939_fmi = -1;
static int hf_j1939_oc = -1;

static int hf_j1939_blockId = -1;

static int proto_j1939 = -1;
static gint ett_j1939 = -1;
static gint ett_j1939_can = -1;
static gint ett_j1939_dtc = -1;
static gint ett_j1939_message = -1;


static gint ett_bam_fragment = -1;
static gint ett_bam_fragments = -1;
static gint hf_bam_fragments = -1;
static gint hf_bam_fragment = -1;
static gint hf_bam_fragment_overlap = -1;
static gint hf_bam_fragment_overlap_conflict = -1;
static gint hf_bam_fragment_multiple_tails = -1;
static gint hf_bam_fragment_too_long_fragment = -1;
static gint hf_bam_fragment_error = -1;
static gint hf_bam_fragment_count = -1;
static gint hf_bam_reassembled_in = -1;
static gint hf_bam_reassembled_length = -1;
static gint hf_bam_reassembled_data = -1;

static dissector_handle_t j1939_handle;

static reassembly_table bam_reassembly_table;


static const fragment_items bam_frag_items = {
  &ett_bam_fragment,
  &ett_bam_fragments,
  &hf_bam_fragments,
  &hf_bam_fragment,
  &hf_bam_fragment_overlap,
  &hf_bam_fragment_overlap_conflict,
  &hf_bam_fragment_multiple_tails,
  &hf_bam_fragment_too_long_fragment,
  &hf_bam_fragment_error,
  &hf_bam_fragment_count,
  &hf_bam_reassembled_in,
  &hf_bam_reassembled_length,
  &hf_bam_reassembled_data,
  "BAM fragments"
};


static const value_string tts_status[] = {
    { 0, "Off" },
    { 1, "Red" },
    { 2, "Yellow" },
	{ 3, "Info" },
	{ 7, "Not available" }
};

BamReassembler bamReassembler;

std::map<u32/*SPN number*/, int/*header_field_info.id*/> spnNumToHinfoId;
std::map<u32/*TTS number*/, int/*header_field_info.id*/> ttsNumToHinfoId;

void proto_register_j1939(void) {

	static hf_register_info hf[] = {
		{ &hf_j1939_frame,
			{"Frame", "j1939.frame",
					FT_STRING, BASE_NONE, NULL, 0x0, NULL, HFILL }
		},
		{ &hf_j1939_spn,
			{"Spn", "j1939.spn",
					FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }
		},
		{ &hf_j1939_dtc,
			{"Diagnosis Trouble Code", "j1939.dtc",
					FT_NONE, BASE_NONE, NULL, 0x0, NULL, HFILL }
		},
		{ &hf_j1939_oc,
			{"Ocurrence Count", "j1939.oc",
					FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL }
		},
		{ &hf_j1939_fmi,
			{"Failure Mode Identifier", "j1939.fmi",
					FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL }
		},
		{ &hf_j1939_blockId,
			{"Block ID", "j1939.fms1.blockId",
					FT_UINT8, BASE_DEC, NULL, 0x0F, NULL, HFILL }
		},
		{ &hf_bam_fragment_overlap,
			      { "Fragment overlap", "bam.fragment.overlap", FT_BOOLEAN, BASE_NONE,
			        NULL, 0x0, "Fragment overlaps with other fragments", HFILL }},

		{ &hf_bam_fragment_overlap_conflict,
		  { "Conflicting data in fragment overlap", "bam.fragment.overlap.conflict",
			FT_BOOLEAN, BASE_NONE, NULL, 0x0,
			"Overlapping fragments contained conflicting data", HFILL }},

		{ &hf_bam_fragment_multiple_tails,
		  { "Multiple tail fragments found", "bam.fragment.multipletails",
			FT_BOOLEAN, BASE_NONE, NULL, 0x0,
			"Several tails were found when defragmenting the packet", HFILL }},

		{ &hf_bam_fragment_too_long_fragment,
		  { "Fragment too long", "bam.fragment.toolongfragment",
			FT_BOOLEAN, BASE_NONE, NULL, 0x0,
			"Fragment contained data past end of packet", HFILL }},

		{ &hf_bam_fragment_error,
		  { "Defragmentation error", "bam.fragment.error", FT_FRAMENUM, BASE_NONE,
			NULL, 0x0, "Defragmentation error due to illegal fragments", HFILL }},

		{ &hf_bam_fragment_count,
		  { "Fragment count", "bam.fragment.count", FT_UINT32, BASE_DEC,
			NULL, 0x0, NULL, HFILL }},

		{ &hf_bam_fragment,
		  { "BAM Fragment", "bam.fragment", FT_FRAMENUM, BASE_NONE,
			NULL, 0x0, NULL, HFILL }},

		{ &hf_bam_fragments,
		  { "BAM Fragments", "bam.fragments", FT_BYTES, BASE_NONE,
			NULL, 0x0, NULL, HFILL }},

		{ &hf_bam_reassembled_in,
		  { "Reassembled BAM in frame", "bam.reassembled_in", FT_FRAMENUM, BASE_NONE,
		  NULL, 0x0, "This BAM packet is reassembled in this frame", HFILL }},

		{ &hf_bam_reassembled_length,
		  { "Reassembled BAM length", "bam.reassembled.length", FT_UINT32, BASE_DEC,
			NULL, 0x0, "The total length of the reassembled payload", HFILL }},

		{ &hf_bam_reassembled_data,
		  { "Reassembled BAM data", "bam.reassembled.data", FT_BYTES, BASE_NONE,
			NULL, 0x0, "The reassembled payload", HFILL }}
	};

	static gint *ett[] = {
		&ett_j1939,
		&ett_j1939_can,
		&ett_j1939_dtc,
		&ett_j1939_message,
		&ett_bam_fragment,
		&ett_bam_fragments
	};

	reassembly_table_register(&bam_reassembly_table,
	                        &addresses_reassembly_table_functions);

	proto_j1939 = proto_register_protocol("j1939framework Protocol",
											"j1939framework",
											"j1939framework");

	proto_register_field_array(proto_j1939, hf, array_length(hf));
	proto_register_subtree_array(ett, array_length(ett));

}

void proto_reg_handoff_j1939(void) {


	j1939_handle = create_dissector_handle( dissect_J1939, proto_j1939 );

	//Load database
	J1939DataBase ddbb;
	if(!ddbb.parseJsonFile(DATABASE_PATH)) {		//Something went wrong
		return;
	}

	const std::vector<GenericFrame>& ddbbFrames = ddbb.getParsedFrames();


	//Register all the frames listed in the database
	for(auto iter = ddbbFrames.begin(); iter != ddbbFrames.end(); ++iter) {

		J1939Factory::getInstance().registerFrame(*iter);

	}

	header_field_info* info;
	std::string abbrev;
	std::string tts_name;

	for(unsigned int tts = 1; tts <= NUMBER_OF_BLOCKS * TTSS_PER_BLOCK; ++tts) {

		info = (header_field_info*)g_malloc0(sizeof(header_field_info));

		info->id = -1;
		info->ref_type = HF_REF_TYPE_NONE;
		info->same_name_prev_id = -1;

		abbrev = std::string("j1939.tts.") + std::to_string(tts);
		tts_name = std::string("TTS ") + std::to_string(tts) + std::string(" Status");

		info->name = g_strdup(tts_name.c_str());
		info->abbrev = g_strdup(abbrev.c_str());

		info->type = FT_UINT8;
		info->display = BASE_DEC;

		info->strings = VALS(tts_status);

		info->bitmask = 0x7;

		if((((tts - 1) % TTSS_PER_BLOCK) % 2) == 0) {				//If TTS number is even, the bitmask corresponds to the 4 most significant bits. Otherwise, it corresponds to the 4 least significant bits.
			info->bitmask <<= 4;
		}

		proto_register_fields_section(proto_j1939, info, 1);

		ttsNumToHinfoId[tts] = info->id;

	}

	std::set<u32> pgns = J1939Factory::getInstance().getAllRegisteredPGNs();

	

	//Register dissectors for all the known frames
	for(auto pgn = pgns.begin(); pgn != pgns.end(); ++pgn) {

		dissector_add_uint("j1939.pgn", *pgn, j1939_handle);

		std::unique_ptr<J1939Frame> frame = J1939Factory::getInstance().getJ1939Frame(*pgn);

		if(!frame->isGenericFrame()) {
			continue;
		}

		GenericFrame *genFrame = static_cast<GenericFrame *>(frame.get());

		//Also, we have to register SPN specific fields
		std::set<u32> spnNumbers = genFrame->getSPNNumbers();

		for(auto spnNumber = spnNumbers.begin(); spnNumber != spnNumbers.end(); ++spnNumber) {

			const SPN* spn = genFrame->getSPN(*spnNumber);

			info = (header_field_info*)g_malloc0(sizeof(header_field_info));

			info->id = -1;
			info->ref_type = HF_REF_TYPE_NONE;
			info->same_name_prev_id = -1;

			abbrev = std::string("j1939.spn.") + std::to_string(*spnNumber);

			info->name = g_strdup(spn->getName().c_str());
			info->abbrev = g_strdup(abbrev.c_str());

			switch(spn->getType()) {

			case SPN::SPN_STATUS: {

				info->type = FT_UINT8;
				info->display = BASE_DEC;

			}	break;

			case SPN::SPN_NUMERIC: {

				info->type = FT_DOUBLE;
				info->display = BASE_NONE;

			}	break;

			case SPN::SPN_STRING: {

				info->type = FT_STRING;
				info->display = BASE_NONE;

			}	break;

			default:
				break;

			}

			proto_register_fields_section(proto_j1939, info, 1);

			spnNumToHinfoId[*spnNumber] = info->id;

		}



	}

}


int dissect_J1939(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree, void*) {


	proto_item *ti;
	proto_tree *j1939_tree;

	//Get the buffer from the upper layer so that we can obtain the ID for the Can Frame
	tvbuff_t *canIdTvb = tvb_get_ds_tvb(tvb);
	
	//Obtain the length of the concerning frame
	guint32 data_length = tvb_reported_length(tvb);

	//Allocate a buffer where to store the DLC (payload of the can frame)
	guint8* content = (guint8*)wmem_alloc(pinfo->pool, data_length);

	//Obtain the can id
	
	guint32 canId;

	if(tvb_reported_length(canIdTvb) >= 20) {		//Not reassembled frames
		guint32 leCanId = tvb_get_ntohl(canIdTvb, 0x10);
		canId = (leCanId >> 24) | ((leCanId >> 8)&0xFF00) | ((leCanId << 8)&0xFF0000) | ((leCanId << 24) &0xFF000000);
	} else if(tvb_reported_length(canIdTvb) >= 4) {		//For reassembled frames
		canId = tvb_get_ntohl(canIdTvb, 0);
	} else {
		std::cout << "tvb buffer too short" << std::endl;
		return -1;	
	}

	canId &= 0x1FFFFFFF;
	
	
	
	//Obtain the payload of the Can Frame
	tvb_memcpy(tvb, content, 0, data_length);

	//Generate the frame from the frame factory. As arguments, we pass the ID, the data and the length.
	std::unique_ptr<J1939Frame> frame = J1939Factory::getInstance().getJ1939Frame(canId, content, data_length);


	if(!frame.get()) {			//Bad decoding...
		std::cout << "dissect_J1939 error" << std::endl;
		return -1;
	}

	col_set_str(pinfo->cinfo, COL_PROTOCOL, "J1939");
	col_clear(pinfo->cinfo, COL_INFO);

	//In the info column, publish the name of the frame
	col_add_fstr(pinfo->cinfo, COL_INFO, "Frame: %s", frame->getName().c_str());

	//Add item to the tree with the name of the frame
	ti = proto_tree_add_string(tree, hf_j1939_frame, canIdTvb, 1, 2, frame->getName().c_str());
	j1939_tree = proto_item_add_subtree(ti, ett_j1939);

	if(frame->getPGN() == DM1_PGN) {
		DM1 *dm1Frame = static_cast<DM1 *>(frame.get());

		dissect_dm1_frame(tvb, j1939_tree, ti, dm1Frame);
	}


	if(frame->isGenericFrame()) {		//If the frame is a generic frame (this means that it contains several defined SPNs), call a proper function to handle it
		GenericFrame * genFrame = (GenericFrame *)(frame.get());
		dissect_generic_frame(tvb, j1939_tree, ti, genFrame);


	} else if(bamReassembler.toBeHandled(*frame)) {		//This frame is part of BAM protocol


		if(tvb_get_guint8(canIdTvb, 1) == 1) {
			return 0;		//Broadcast frames are not reassembled
		}

		//We let our reassembler to handle them
		guint expected_size = 0;

		fragment_head *bam_head = NULL;

		tvbuff_t *def_tvb = NULL;
		tvbuff_t *next_tvb;

		guint sq = 0;
		gboolean update_col_info = TRUE;
		guint32 frag_data_len = 0;

		if(!pinfo->fd->visited) {		//Only treat the frames if it is the first time we see them!!!

			try {

				expected_size = bamReassembler.handleFrame(*frame);

			} catch(J1939EncodeException &) {

				return -1;

			}


			switch(frame->getPGN()) {

			case TP_CM_PGN: {

				TPCMFrame *connFrame = (TPCMFrame *)(frame.get());

				guint8 *canIdTVBData = (guint8 *)g_malloc(sizeof(guint32));

				guint32 realCanId = (connFrame->getPriority() << J1939_PRIORITY_OFFSET)
						| (connFrame->getDataPgn() << J1939_PGN_OFFSET)
						| (connFrame->getSrcAddr() << J1939_SRC_ADDR_OFFSET);


				frag_data_len = sizeof(guint32);

				phton32(canIdTVBData, realCanId);

				def_tvb = tvb_new_real_data(canIdTVBData, frag_data_len, frag_data_len);

				tvb_set_free_cb(def_tvb, g_free);

			}	break;


			case TP_DT_PGN: {

				TPDTFrame *dataFrame = (TPDTFrame *)(frame.get());

				sq = dataFrame->getSq();

				frag_data_len = TP_DT_PACKET_SIZE;

				gint trailing_bytes = TP_DT_PACKET_SIZE * sq - expected_size;

				if(trailing_bytes >= TP_DT_PACKET_SIZE) {
					return -1;
				}

				if(trailing_bytes > 0) {
					frag_data_len -= trailing_bytes;
				}

				def_tvb = tvb_new_subset_remaining(tvb, 1);


			}	break;

			default:
				break;

			}

		}


		bam_head = fragment_add_seq_check(&bam_reassembly_table, def_tvb, 0,
		   pinfo, 0,
		   NULL, sq,
		   frag_data_len, !bamReassembler.reassembledFramesPending());


		if(bamReassembler.reassembledFramesPending()) {
			bamReassembler.dequeueReassembledFrame();
		}

		next_tvb = process_reassembled_data(tvb, 0, pinfo, "Reassembled BAM",
					bam_head, &bam_frag_items, &update_col_info, j1939_tree);


		if(next_tvb) {

			call_dissector(j1939_handle, tvb_new_subset_remaining(next_tvb, sizeof(guint32)),
				    pinfo, j1939_tree);
		}

	} else {		//Another type of frame than the generic ones and the ones that belong to BAM protocol

		switch(frame->getPGN()) {
		case FMS1_PGN:
		{

			FMS1Frame* fms1 = (FMS1Frame *)(frame.get());

			dissect_fms1_frame(tvb, j1939_tree, ti, fms1);

		}	break;
		default:
			break;
		}
	}

	return tvb_captured_length(tvb);

}

void dissect_generic_frame(tvbuff_t *tvb, proto_tree *j1939_tree, proto_item *ti, GenericFrame * genFrame) {

	proto_tree *spn_tree;
	std::set<guint32> spnNumbers = genFrame->getSPNNumbers();

	for(auto iter = spnNumbers.begin(); iter != spnNumbers.end(); ++iter) {

		const SPN *spn = genFrame->getSPN(*iter);

		ti = proto_tree_add_uint(j1939_tree, hf_j1939_spn, tvb, spn->getOffset(), spn->getByteSize(), *iter);
		spn_tree = proto_item_add_subtree(ti, ett_j1939_can);

		switch(spn->getType()) {
		case SPN::SPN_NUMERIC: {
			const SPNNumeric *spnNum = (SPNNumeric *)(spn);

			ti = proto_tree_add_double_format(spn_tree, spnNumToHinfoId[*iter], tvb, spn->getOffset(),
					spnNum->getByteSize(), spnNum->getFormattedValue(), "%s: %.10g %s",
					spn->getName().c_str(), spnNum->getFormattedValue(), spnNum->getUnits().c_str());

		}	break;
		case SPN::SPN_STATUS: {

			const SPNStatus *spnStatus = (SPNStatus *)(spn);

			ti = proto_tree_add_uint_bits_format_value(spn_tree, spnNumToHinfoId[*iter], tvb,
					(spn->getOffset() << 3) + 8 - spnStatus->getBitOffset() - spnStatus->getBitSize()/* + spnStatus->getBitOffset()*/,
					spnStatus->getBitSize(), spnStatus->getValue(), "%s (%u)"
					, spnStatus->getValueDescription(spnStatus->getValue()).c_str(),
					spnStatus->getValue());


		}	break;

		case SPN::SPN_STRING: {

			const SPNString *spnStr = (SPNString *)(spn);

			ti = proto_tree_add_item(spn_tree, spnNumToHinfoId[*iter], tvb, spn->getOffset(), spnStr->getValue().size(), ENC_NA);

		}	break;
		default:
			break;
		}

	}

}


void dissect_fms1_frame(tvbuff_t *tvb, proto_tree *j1939_tree, proto_item *, const FMS1Frame *fms1Frame) {

	u8 firstTts = TTSS_PER_BLOCK * fms1Frame->getBlockID() + 1;

	proto_tree_add_item(j1939_tree, hf_j1939_blockId, tvb, 0, 1, ENC_NA);

	for(int tts = firstTts; tts < firstTts + TTSS_PER_BLOCK; ++tts) {

		proto_tree_add_item(j1939_tree, ttsNumToHinfoId[tts], tvb, (((tts - 1) % TTSS_PER_BLOCK) + 1)/ 2, 1, ENC_NA);
	}

}


void dissect_dm1_frame(tvbuff_t *tvb, proto_tree *j1939_tree, proto_item *ti, const DM1 *dm1Frame) {

	const std::vector<DTC>& dtcs = dm1Frame->getDTCs();

	proto_tree *dtc_tree;

	size_t offset = 2;

	for(auto dtc = dtcs.begin(); dtc != dtcs.end(); ++dtc) {

		ti = proto_tree_add_item(j1939_tree, hf_j1939_dtc, tvb, offset, DTC_SIZE, ENC_NA);
		dtc_tree = proto_item_add_subtree(ti, ett_j1939_dtc);

		ti = proto_tree_add_uint_bits_format_value(dtc_tree, hf_j1939_spn, tvb,
			(offset << 3),
			SPN_NUMBER_MAX_BITS, dtc->getSpn(), "%u"
			, dtc->getSpn());

		ti = proto_tree_add_uint_bits_format_value(dtc_tree, hf_j1939_fmi, tvb,
					(offset << 3) + SPN_NUMBER_MAX_BITS,
					5, dtc->getFmi(), "%u"
					, dtc->getFmi());

		ti = proto_tree_add_uint_bits_format_value(dtc_tree, hf_j1939_oc, tvb,
							(offset << 3) + SPN_NUMBER_MAX_BITS + 5 + 1,
							7, dtc->getOc(), "%u"
							, dtc->getOc());



		offset += DTC_SIZE;

	}

}
