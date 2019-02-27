cmake_minimum_required(VERSION 3.5)

project(BinUtils)

#set(PROPERTIES EXCLUDE_FROM_ALL TRUE)

add_subdirectory(j1939Decoder)
add_subdirectory(j1939Sender)
add_subdirectory(j1939Sniffer)
add_subdirectory(TRCDumper)
add_subdirectory(TRCPlayer)
add_subdirectory(TRCToCap)
add_subdirectory(j1939AddrClaim)
add_subdirectory(j1939AddressMapper)
