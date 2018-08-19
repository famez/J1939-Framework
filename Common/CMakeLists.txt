cmake_minimum_required(VERSION 3.5)

project(Common)

add_library(Common STATIC 
    Utils.cpp
)

target_include_directories(Common
    PUBLIC 
        include
)


install (TARGETS Common
    DESTINATION lib)


install(DIRECTORY include/ 
    DESTINATION include)
