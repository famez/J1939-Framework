cmake_minimum_required(VERSION 3.5)

project(Common)

add_library(Common STATIC 
    Utils.cpp
)

target_include_directories(Common
    PRIVATE 
        include
)


install (TARGETS Common EXPORT J1939FrameworkTargets
    DESTINATION lib)


install(DIRECTORY include/ 
    DESTINATION include)
