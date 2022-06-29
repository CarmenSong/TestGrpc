#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "libupdate.cpp"

void Runturtle(int a, int b) {
        
        auto node = rclcpp::Node::make_shared("turtle_controller");
        auto publisher = node->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel",1);

        while (rclcpp::ok()) {
                auto msg = geometry_msgs::msg::Twist();
                msg.linear.x = a;
                msg.angular.z = b;
                publisher->publish(msg);

                lib_management();
                if (roslib_update) {
                        roslib_update = false;
                        break;
                }

        }

        return;
}
