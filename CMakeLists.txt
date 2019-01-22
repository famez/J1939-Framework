cmake_minimum_required(VERSION 3.5)

project(J1939Framework)

#set(CMAKE_BUILD_TYPE Debug)    #For debug purposes

set(J1939Framework_VERSION 1.0.0)

set(CMAKE_MODULE_PATH "./cmake/;${CMAKE_MODULE_PATH}")


find_package (Threads REQUIRED)
find_package (jsoncpp REQUIRED)
find_package (LibWebSockets REQUIRED)


set (CMAKE_CXX_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_definitions(-DDATABASE_PATH="${CMAKE_INSTALL_PREFIX}/etc/j1939/frames.json")

add_subdirectory(Common)
add_subdirectory(CAN)
add_subdirectory(J1939)
add_subdirectory(Database)
add_subdirectory(BinUtils)
add_subdirectory(GUI_WEB/backend)
add_subdirectory(j1939AddressClaimer)


include(GenerateExportHeader)

generate_export_header(J1939)
set_property(TARGET J1939 PROPERTY VERSION ${J1939Framework_VERSION})
set_property(TARGET J1939 PROPERTY SOVERSION 1)
set_property(TARGET J1939 PROPERTY INTERFACE_J1939_MAJOR_VERSION 1)
set_property(TARGET J1939 APPEND PROPERTY
  COMPATIBLE_INTERFACE_STRING J1939MAJOR_VERSION
)


include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/J1939FrameworkConfigVersion.cmake"
  VERSION ${J1939Framework_VERSION}
	COMPATIBILITY AnyNewerVersion
)

export(EXPORT J1939FrameworkTargets
  FILE "${CMAKE_CURRENT_BINARY_DIR}/J1939FrameworkTargets.cmake"
  NAMESPACE J1939::
)


configure_file(cmake/J1939FrameworkConfig.cmake
  "${CMAKE_CURRENT_BINARY_DIR}/J1939FrameworkConfig.cmake"
COPYONLY
  
)

set(ConfigPackageLocation lib/cmake/J1939Framework)
install(EXPORT J1939FrameworkTargets
  FILE
   J1939FrameworkTargets.cmake
  NAMESPACE
    J1939::
  DESTINATION
    ${ConfigPackageLocation}
)

install(
  FILES
    cmake/J1939FrameworkConfig.cmake
    "${CMAKE_CURRENT_BINARY_DIR}/J1939FrameworkConfigVersion.cmake"
  DESTINATION
    ${ConfigPackageLocation}
  COMPONENT
    Devel
)
