cmake_minimum_required(VERSION 3.5)

project(j1939GUI)

set (GUI_SERVER_DIR "/var/www/j1939gui")

add_definitions(-DHTTP_DIR="${GUI_SERVER_DIR}")

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS graph.proto)

add_executable(j1939GUI 
    src/J1939GUI.cpp
    src/graph.cpp
    ${PROTO_SRCS}
    ${PROTO_HDRS}
)


#Visible for frontend
configure_file(graph.proto ../frontend/graph.proto COPYONLY)

target_include_directories(j1939GUI
    PUBLIC 
        . ${J1939_SOURCE_DIR}/include ${Can_SOURCE_DIR}/include ${Common_SOURCE_DIR}/include ${LIBWEBSOCKETS_INCLUDE_DIR} ${Protobuf_INCLUDE_DIRS}
)

link_directories(${LIBWEBSOCKETS_LIBRARY_DIR})


target_link_libraries(j1939GUI
    PUBLIC
        J1939 Can rt -rdynamic jsoncpp ${LIBWEBSOCKETS_LIBRARIES} ${Protobuf_LIBRARIES}
)

install (TARGETS j1939GUI
    DESTINATION bin)

install (DIRECTORY ../frontend/
	DESTINATION ${GUI_SERVER_DIR})
    