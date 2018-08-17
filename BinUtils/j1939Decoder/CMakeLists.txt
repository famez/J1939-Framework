cmake_minimum_required(VERSION 3.5)

project(j1939Decoder)

add_executable(j1939Decoder 
    src/j1939Decoder.cpp
)

target_include_directories(j1939Decoder
    PUBLIC 
        . ${J1939_SOURCE_DIR} ${Common_SOURCE_DIR}
)

target_link_libraries(j1939Decoder
    PUBLIC
        J1939
)

install (TARGETS j1939Decoder
    DESTINATION bin)
