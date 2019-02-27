cmake_minimum_required(VERSION 3.5)

project(TRCPlayer)

add_executable(TRCPlayer 
    src/TRCPlayer.cpp
)

target_include_directories(TRCPlayer
    PUBLIC 
        include ${Can_SOURCE_DIR}/include ${Common_SOURCE_DIR}/include
)

target_link_libraries(TRCPlayer
    PUBLIC
        Can dl rt
)


install (TARGETS TRCPlayer
    DESTINATION bin)
