cmake_minimum_required(VERSION 3.19)
project(test C CXX)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)

add_library(dlopenRos SHARED src/RunTurtle.cpp)
ament_target_dependencies(dlopenRos rclcpp geometry_msgs)

#target_compile_options(dlopenRos PUBLIC -Wall)
#ament_target_dependencies(dlopenRos rclcpp geometry_msgs)
#target_include_directories(dlopenRos
#        PUBLIC
#                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#                $<INSTALL_INTERFACE:include>
#)

add_executable(main src/main.cpp)
ament_target_dependencies(main rclcpp geometry_msgs)
target_link_libraries(main dlopenRos)