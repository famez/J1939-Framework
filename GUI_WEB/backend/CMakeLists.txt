cmake_minimum_required(VERSION 3.5)

project(j1939GUI)

add_executable(j1939GUI 
    src/J1939GUI.cpp
)

target_include_directories(j1939GUI
    PUBLIC 
        include ${J1939_SOURCE_DIR}/include ${Can_SOURCE_DIR}/include ${Common_SOURCE_DIR}/include
)

target_link_libraries(j1939GUI
    PUBLIC
        J1939 Can rt -rdynamic jsoncpp websockets
)


install (TARGETS j1939GUI
    DESTINATION bin)
