cmake_minimum_required(VERSION 3.5)

project(TRCToCap)

find_package (PkgConfig)

if(PKGCONFIG_FOUND)

pkg_check_modules (GLIB2 glib-2.0)

if(GLIB2_FOUND)

message("-- GLIB2 is available")
message("-- Project " ${PROJECT_NAME} " will be built")

add_executable(TRCToCap 
    src/TRCToCap.cpp
    src/pcapio.c
)

target_include_directories(TRCToCap
    PUBLIC 
        . ${Can_SOURCE_DIR} ${Common_SOURCE_DIR} ${GLIB2_INCLUDE_DIRS}
)

target_link_libraries(TRCToCap
    PUBLIC
        Can
)


install (TARGETS TRCToCap
    DESTINATION bin)

else(GLIB2_FOUND)

message(WARNING "-- GLIB2 is not available")
message(WARNING "-- Project " ${CMAKE_PROJECT_NAME} " is skipped...")
message(WARNING "-- Try to install GLIB2 and run cmake again...")

endif(GLIB2_FOUND)
endif(PKGCONFIG_FOUND)
