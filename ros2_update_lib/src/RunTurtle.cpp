#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"

void Runturtle(int a, int b) {
        
        auto node = rclcpp::Node::make_shared("turtle_controller");
        auto publisher = node->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel",1);
        std::cout << "Update" << std::endl;
        while (rclcpp::ok()) {
                auto msg = geometry_msgs::msg::Twist();
                msg.linear.x = 10*a;
                msg.angular.z = 5*b;
                publisher->publish(msg);
               

        }

        return;
}
