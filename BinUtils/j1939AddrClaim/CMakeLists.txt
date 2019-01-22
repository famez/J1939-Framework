cmake_minimum_required(VERSION 3.5)

project(j1939AddrClaim)

add_executable(j1939AddrClaim 
    j1939AddrClaim.cpp
)

target_include_directories(j1939AddrClaim
    PUBLIC 
        include ${J1939_SOURCE_DIR}/include ${Can_SOURCE_DIR}/include ${Common_SOURCE_DIR}/include ${J1939AddressClaimer_SOURCE_DIR}/include
)

target_link_libraries(j1939AddrClaim
    PUBLIC
        J1939 Can J1939AddressClaimer rt jsoncpp -rdynamic
)

install (TARGETS j1939AddrClaim
    DESTINATION bin)
