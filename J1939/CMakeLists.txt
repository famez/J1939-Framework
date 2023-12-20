cmake_minimum_required(VERSION 3.5)

project(J1939)

add_library(J1939
	./J1939Common.cpp
	./Diagnosis/Frames/DM1.cpp
	./Diagnosis/DTC.cpp
	./FMS/TellTale/TellTale.cpp
	./FMS/TellTale/FMS1Frame.cpp
	./J1939Factory.cpp
	./GenericFrame.cpp
	./SPN/SPN.cpp
	./SPN/SPNString.cpp
	./SPN/SPNStatus.cpp
	./SPN/SPNNumeric.cpp
	./SPN/SPNSpec/SPNSpec.cpp
	./SPN/SPNSpec/SPNNumericSpec.cpp
	./SPN/SPNSpec/SPNStatusSpec.cpp
	./SPN/SPNHistory.cpp
	./J1939DataBase.cpp
	./J1939Frame.cpp
	./Addressing/AddressClaimFrame.cpp
	./Frames/RequestFrame.cpp
	./Transport/TPCMFrame.cpp
	./Transport/BAM/BamReassembler.cpp
	./Transport/BAM/BamFragmenter.cpp
	#./Transport/RTSCTS/RTSCTSConnectionManager.cpp
	./Transport/TPDTFrame.cpp

)

target_include_directories(J1939
    PRIVATE 
        include ${Common_SOURCE_DIR}/include
)

target_link_libraries(J1939
    PUBLIC
        Common jsoncpp
)

install (TARGETS J1939 EXPORT J1939FrameworkTargets
    LIBRARY DESTINATION lib)


install(DIRECTORY include/ 
    DESTINATION include
COMPONENT
    Devel
)
