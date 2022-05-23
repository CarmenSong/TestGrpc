cmake_minimum_required(VERSION 3.5.1)

project(TestRos C CXX)

include(../cmake/common.cmake)

get_filename_component(ros_proto "../../protos/TestRos.proto" ABSOLUTE)
get_filename_component(ros_proto_path "${ros_proto}" PATH)

set(ros_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/TestRos.pb.cc")
set(ros_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/TestRos.pb.h")
set(ros_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/TestRos.grpc.pb.cc")
set(ros_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/TestRos.grpc.pb.h")
add_custom_command(
        OUTPUT "${ros_proto_srcs}" "${ros_proto_hdrs}" "${ros_grpc_srcs}" "${ros_grpc_hdrs}"
        COMMAND ${_PROTOBUF_PROTOC}
        ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${ros_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${ros_proto}")

include_directories("${CMAKE_CURRENT_BINARY_DIR}")

add_library(ros_grpc_proto
        ${ros_grpc_srcs}
        ${ros_grpc_hdrs}
        ${ros_proto_srcs}
        ${ros_proto_hdrs})
target_link_libraries(
        ros_grpc_proto
        ${_REFLECTION}
        ${_GRPC_GRPCPP}
        ${_PROTOBUF_LIBPROTOBUF})

foreach(
        _target
        ros_client ros_server)
        add_executable(${_target} "${_target}.cc")
        target_link_libraries(${_target}
        ros_grpc_proto
        ${_REFLECTION}
        ${_GRPC_GRPCPP}
        ${_PROTOBUF_LIBPROTOBUF}
        )
endforeach()