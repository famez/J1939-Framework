cmake_minimum_required(VERSION 3.5)

project(J1939Framework)

find_package (Threads REQUIRED)

set (CMAKE_CXX_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_definitions(-DDATABASE_PATH="${CMAKE_INSTALL_PREFIX}/etc/j1939/frames.json")

add_subdirectory(Common)
add_subdirectory(CAN)
add_subdirectory(J1939)
add_subdirectory(Database)
add_subdirectory(BinUtils)
