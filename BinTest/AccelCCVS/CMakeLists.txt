cmake_minimum_required(VERSION 3.5)

project(accelCCVS)

set(CMAKE_BUILD_TYPE Debug)    #For debug purposes

find_package(J1939Framework REQUIRED)

add_definitions(-DDATABASE_PATH="${J1939_Database}")

set(CMAKE_CXX_STANDARD 11)

add_executable(accelCCVS 
    src/accelCCVS.cpp
)


target_link_libraries(accelCCVS
    PUBLIC
        J1939 Can rt -rdynamic
)


install (TARGETS accelCCVS
    DESTINATION bin)
