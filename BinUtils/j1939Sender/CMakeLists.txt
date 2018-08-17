cmake_minimum_required(VERSION 3.5)

project(j1939Sender)

add_executable(j1939Sender 
    src/j1939Sender.cpp
)

target_include_directories(j1939Sender
    PUBLIC 
        . ${J1939_SOURCE_DIR} ${Can_SOURCE_DIR} ${Common_SOURCE_DIR}
)

target_link_libraries(j1939Sender
    PUBLIC
        J1939 Can rt -rdynamic
)


install (TARGETS j1939Sender
    DESTINATION bin)
