# CMakeLists.txt
#
# Wireshark - Network traffic analyzer
# By Gerald Combs <gerald@wireshark.org>
# Copyright 1998 Gerald Combs
#
# SPDX-License-Identifier: GPL-2.0-or-later
#

cmake_minimum_required(VERSION 3.5)

include(WiresharkPlugin)

find_package(J1939Framework REQUIRED)

add_definitions(-DDATABASE_PATH="${J1939_Database}")

# Plugin name and version info (major minor micro extra)
set_module_info(j1939 0 0 1 0)

set(DISSECTOR_SRC
	packet-j1939.cpp
)

set(PLUGIN_FILES
	plugin.c
	${DISSECTOR_SRC}
)

set_source_files_properties(
	${PLUGIN_FILES}
	PROPERTIES
	COMPILE_FLAGS "${WERROR_COMMON_FLAGS}"
)

set (CMAKE_CXX_STANDARD 11)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

register_plugin_files(plugin.c
	plugin
	${DISSECTOR_SRC}
)

add_plugin_library(j1939 epan)

target_link_libraries(j1939 epan J1939 -rdynamic)

install_plugin(j1939 epan)

file(GLOB DISSECTOR_HEADERS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.h")
CHECKAPI(
	NAME
	  j1939
	SWITCHES
	  -g abort -g termoutput -build
	SOURCES
	  ${DISSECTOR_SRC}
	  ${DISSECTOR_HEADERS}
)

#
# Editor modelines  -  http://www.wireshark.org/tools/modelines.html
#
# Local variables:
# c-basic-offset: 8
# tab-width: 8
# indent-tabs-mode: t
# End:
#
# vi: set shiftwidth=8 tabstop=8 noexpandtab:
# :indentSize=8:tabSize=8:noTabs=false:
#
