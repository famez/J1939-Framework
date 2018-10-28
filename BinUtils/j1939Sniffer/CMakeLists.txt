cmake_minimum_required(VERSION 3.5)

project(j1939Sniffer)

find_package (Curses)

if(CURSES_FOUND)

message("-- NCURSES is available")

message("-- Project " ${PROJECT_NAME} " will be built")

add_executable(j1939Sniffer 
    src/j1939Sniffer.cpp
)

target_include_directories(j1939Sniffer
    PUBLIC 
        include ${J1939_SOURCE_DIR}/include ${Can_SOURCE_DIR}/include ${Common_SOURCE_DIR}/include
)

target_link_libraries(j1939Sniffer
    PUBLIC
        J1939 Can ncurses -rdynamic jsoncpp
)


install (TARGETS j1939Sniffer
    DESTINATION bin)

else(CURSES_FOUND)

message(WARNING "-- NCURSES is not available")
message(WARNING "-- Project " ${CMAKE_PROJECT_NAME} " is skipped...")
message(WARNING "-- Try to install ncurses and run cmake again...")

endif(CURSES_FOUND)
