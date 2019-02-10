cmake_minimum_required(VERSION 3.5)

# Locate GTest
find_package(GTest REQUIRED)


include_directories(
			include
			${GTEST_INCLUDE_DIRS}
			${J1939_SOURCE_DIR}/include 
			${Common_SOURCE_DIR}/include 
			${J1939AddressClaimer_SOURCE_DIR}/include
			)
 
add_executable(execTests 
			main.cpp 
			TestFrame.cpp
			SPNNumeric_test.cpp
			SPNStatus_test.cpp
			SPNString_test.cpp
			genericframe_test.cpp
			j1939Factory_test.cpp
			database_test.cpp
			BAM_test.cpp
			)
			
			
target_link_libraries(execTests 
			${GTEST_LIBRARIES} 
			pthread
			J1939 
			J1939AddressClaimer 
			rt 
			jsoncpp 
			-rdynamic
			)
