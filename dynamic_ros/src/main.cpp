#include <iostream>

#include "dlfcn.h"
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"

int main (int argc, char **argv) {

        rclcpp::init(argc, argv);

        const auto dlopenRos = dlopen("./libdlopenRos.so", RTLD_NOW);

        auto Moveturtle = reinterpret_cast<void(*)(int, int)>(dlsym(dlopenRos, "_Z9Runturtleii"));

        //int a = 1;
        //int b = 1;

        Moveturtle(1, 1);

        dlclose(dlopenRos);

        rclcpp::shutdown();

        return 0;
}
