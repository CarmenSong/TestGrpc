cmake_minimum_required(VERSION 3.8)
PROJECT (mosquitto_example)
 
# Find required protobuf package, if your find_package protobuf not work, try uncomment these codes
#set(protobuf_root "/home/zhangshuai/miniconda3/envs/pp_py36")
#set(Protobuf_INCLUDE_DIR "${protobuf_root}/include")
#set(Protobuf_LIBRARY "${protobuf_root}/lib/libprotobuf.so")

#set(protobuf_MODULE_COMPATIBLE TRUE)
#find_package(Protobuf REQUIRED)
#message(STATUS "Using protobuf ${protobuf_VERSION}")

include(cmake/common.cmake)
# Find mosquitto 
set(MOSQUITTO_ROOT "scripts/mosquitto/usr/local")
include(cmake/findmosquitto.cmake)
find_mosquitto()

include_directories(${PROTOBUF_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${MOSQUITTO_INCLUDE_DIR})

get_filename_component(msg_proto "/home/jessewl/work/C-Programs/gRPC-mqtt/proto/Message.proto" ABSOLUTE)
get_filename_component(msg_proto_path "${msg_proto}" PATH)

#PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS "${msg_proto}")
set(msg_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/Message.pb.cc")
set(msg_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/Message.pb.h")
set(msg_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/Message.grpc.pb.cc")
set(msg_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/Message.grpc.pb.h")
add_custom_command(
        OUTPUT "${msg_proto_srcs}" "${msg_proto_hdrs}" "${msg_grpc_srcs}" "${msg_grpc_hdrs}"
        COMMAND ${_PROTOBUF_PROTOC}
        ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${msg_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${msg_proto}")

ADD_EXECUTABLE(sub_callback src/sub_callback.cpp ${msg_proto_srcs} ${msg_proto_hdrs} ${msg_grpc_srcs} ${msg_grpc_hdrs})
target_link_libraries(sub_callback ${_PROTOBUF_LIBPROTOBUF} ${MOSQUITTO_LIBRARIES} ${_REFLECTION} ${_GRPC_GRPCPP})

ADD_EXECUTABLE(pub_simple src/pub_simple.cpp ${msg_proto_srcs} ${msg_proto_hdrs} ${msg_grpc_srcs} ${msg_grpc_hdrs})
target_link_libraries(pub_simple ${_PROTOBUF_LIBPROTOBUF} ${MOSQUITTO_LIBRARIES} ${_REFLECTION} ${_GRPC_GRPCPP})