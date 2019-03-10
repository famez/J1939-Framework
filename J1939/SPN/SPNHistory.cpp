/*
 * SPNHistory.cpp
 *
 *  Created on: 12 Feb 2019
 *      Author: fernando
 */

#include <algorithm>

#include <SPN/SPNHistory.h>
#include <SPN/SPNNumeric.h>
#include <SPN/SPNStatus.h>

#define MAX_NUMBER_OF_SAMPLES		50000

namespace J1939 {

SPNHistory::SPNHistory() {
	mSamples.reserve(MAX_NUMBER_OF_SAMPLES);
}

SPNHistory::~SPNHistory() {
}


void SPNHistory::addSample(const Utils::TimeStamp& timeStamp, const SPN& spn) {

	//We assume that the timestamp is always increasing

	if(mSamples.size() == mSamples.capacity())		return;		//Capacity exceeded

	if(!mGeneralSpec) {		//First time we add a SPN
		mGeneralSpec = spn.getSpec();

		switch(spn.getType()) {

		case SPN::SPN_NUMERIC: {
			const SPNNumeric *spnNum = static_cast<const SPNNumeric *>(&spn);
			mSpecificSpec.numeric = spnNum->getNumericSpec();
		}	break;
		case SPN::SPN_STATUS: {
			const SPNStatus *spnStat = static_cast<const SPNStatus *>(&spn);
			mSpecificSpec.status = spnStat->getStatusSpec();
		}	break;
		default:
			break;

		}
	}

	if(mGeneralSpec != spn.getSpec()) {
		return;		//Not the same spn
	}

	//Build the sample
	Sample sample;

	switch(spn.getType()) {

	case SPN::SPN_NUMERIC: {
		const SPNNumeric *spnNum = static_cast<const SPNNumeric *>(&spn);

		if(!mSamples.empty() && mSamples.back().getNumeric() == spnNum->getFormattedValue())		return;		//Value did not change

		sample = Sample(timeStamp, spnNum->getFormattedValue());
	}	break;
	case SPN::SPN_STATUS: {
		const SPNStatus *spnStat = static_cast<const SPNStatus *>(&spn);

		if(!mSamples.empty() && mSamples.back().getStatus() == spnStat->getValue())					return;		//Value did not change

		sample = Sample(timeStamp, spnStat->getValue());
	}	break;
	default:
		return;		//Unknown, nothing to do
		break;

	}

	mSamples.push_back(sample);

}

std::vector<SPNHistory::Sample> SPNHistory::getWindow(const Utils::TimeStamp& timeStamp, u32 milliseconds, u32 samples) const {

	std::vector<Sample> window;


	if(mSamples.size() < 2)			return window;

	u32 period = milliseconds / samples;


	Utils::TimeStamp current = timeStamp - milliseconds;


	//Get the beginning
	Sample sample(current, static_cast<u8>(0));

	size_t pos = std::upper_bound(mSamples.begin(), mSamples.end(), sample) - mSamples.begin();

	if(pos != 0)	--pos;		//Start with the sample whose timestamp is lower or equals to the beginning of the window


	while(current <= timeStamp) {

		Sample mySample(mSamples[pos]);
		mySample.setTimeStamp(current);		//Modify timestamp


		window.push_back(mySample);

		current = current + period;

		if(pos+1 < mSamples.size() && current >= mSamples[pos+1].getTimeStamp()) {
			++pos;
		}

	}

	return window;
}

} /* namespace J1939 */
