cmake_minimum_required(VERSION 3.5)

project(J1939AddressClaimer)

add_library(J1939AddressClaimer SHARED 
    	./AddressClaimer.cpp
)

target_include_directories(J1939AddressClaimer
    PUBLIC 
        include ${J1939_SOURCE_DIR}/include ${Common_SOURCE_DIR}/include
)

target_link_libraries(J1939AddressClaimer
    PUBLIC
        J1939 pthread
)


install (TARGETS J1939AddressClaimer
    LIBRARY DESTINATION lib)


install(DIRECTORY include/ 
    DESTINATION include
)

