cmake_minimum_required(VERSION 3.5)

project(Can)

add_library(Can SHARED 
    	./CanFrame.cpp
	./TRCWriter.cpp
	./CanSniffer.cpp
	./Backends/Sockets/SocketCanReceiver.cpp
	./Backends/Sockets/SocketCanHelper.cpp
	./Backends/Sockets/SocketCanSender.cpp
	./Backends/PeakCan/PeakCanChannels.cpp
	./Backends/PeakCan/PeakCanReceiver.cpp
	./Backends/PeakCan/PeakCanSender.cpp
	./Backends/PeakCan/PeakCanHelper.cpp
	./Backends/PeakCan/PeakCanSymbols.cpp
	./TRCReader.cpp
	./CommonCanSender.cpp
	./ICanHelper.cpp
	./CommonCanReceiver.cpp
	./CanEasy.cpp
)

target_include_directories(Can
    PUBLIC 
        include ${Common_SOURCE_DIR}/include
)

target_link_libraries(Can
    PUBLIC
        Common pthread dl
)


install (TARGETS Can
    LIBRARY DESTINATION lib)

install(DIRECTORY include/ 
    DESTINATION include)
