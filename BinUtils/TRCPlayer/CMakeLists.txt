cmake_minimum_required(VERSION 3.5)

project(TRCPlayer)

find_package (Curses)

if(CURSES_FOUND)

message("-- NCURSES is available")

message("-- Project " ${PROJECT_NAME} " will be built")


add_executable(TRCPlayer 
    src/TRCPlayer.cpp
)

target_include_directories(TRCPlayer
    PUBLIC 
        include ${J1939_SOURCE_DIR}/include ${Can_SOURCE_DIR}/include ${Common_SOURCE_DIR}/include
)

target_link_libraries(TRCPlayer
    PUBLIC
        J1939 Can ncurses dl rt
)


install (TARGETS TRCPlayer
    DESTINATION bin)


endif(CURSES_FOUND)