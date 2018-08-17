cmake_minimum_required(VERSION 3.5)

project(TRCDumper)

add_executable(TRCDumper 
    src/TRCDumper.cpp
)

target_include_directories(TRCDumper
    PUBLIC 
        . ${Can_SOURCE_DIR} ${Common_SOURCE_DIR}
)

target_link_libraries(TRCDumper
    PUBLIC
        Can dl rt
)


install (TARGETS TRCDumper
    DESTINATION bin)
