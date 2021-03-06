#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "libupdate.cpp"
#include <filesystem>
#include <iostream>

void Runturtle(int a, int b) {
        
        auto node = rclcpp::Node::make_shared("turtle_controller");
        auto publisher = node->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel",1);
        int count = 0;
        lib_management();
        int Initial_Lib_Number = Lib_container.size();
        std::cout << "Initial Lib Number: " << Initial_Lib_Number << std::endl;

        while (rclcpp::ok()) {
                auto msg = geometry_msgs::msg::Twist();
                msg.linear.x = a;
                msg.angular.z = b;
                publisher->publish(msg);
                lib_management();
                int Lib_Number = Lib_container.size();

                if (Initial_Lib_Number != Lib_Number) {
                        //std::cout << Lib_container.size() << std::endl;
                        std::cout << "Sth are coming!" << std::endl;
                        Initial_Lib_Number = Lib_Number;
                        char choice;
                        std::cout << "Waiting thrd1 complete, then enter Y" << std::endl;
                        std::cin >> choice;
                        if (choice == 'Y') break;
                }

        }

        return;
}
