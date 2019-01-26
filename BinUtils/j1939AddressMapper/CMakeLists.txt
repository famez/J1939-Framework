cmake_minimum_required(VERSION 3.5)

project(j1939AddressMapper)

add_executable(j1939AddressMapper 
    j1939AddressMapper.cpp
)

target_include_directories(j1939AddressMapper
    PUBLIC 
        include ${J1939_SOURCE_DIR}/include ${Can_SOURCE_DIR}/include ${Common_SOURCE_DIR}/include
)

target_link_libraries(j1939AddressMapper
    PUBLIC
        J1939 Can rt jsoncpp -rdynamic
)

install (TARGETS j1939AddressMapper
    DESTINATION bin)
