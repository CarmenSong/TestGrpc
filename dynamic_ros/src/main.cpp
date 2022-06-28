#include <iostream>
#include <filesystem>
#include <chrono>
#include <filesystem>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <thread>
#include <memory>

#include "dlfcn.h"
#include "unistd.h"
#include "libupdate.cpp"
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"

bool file_update = false;

void lib_update(std::string initial_ft, std::filesystem::file_time_type ft) {

        while (rclcpp::ok()) {

                bool if_file_update = if_lib_update(initial_ft, ft);
                std::cout << initial_ft << std::endl;

                if (if_file_update) {
                
                std::cout << "Library is updated" << std::endl;
                if_file_update = false;
                ft = std::filesystem::last_write_time("/home/jessewl/work/C-Programs/dynamic_ros/build/test/libdlopenRos.so");
                initial_ft = get_filetime(ft);
                file_update = true;
                //thrd2.join();
                //thrd2(loading_rosnode);

                }

                sleep(1);
        }
}

void loading_rosnode() {
        const auto dlopenRos = dlopen ("./libdlopenRos.so", RTLD_NOW);
        auto Moveturtle = reinterpret_cast<void(*)(int, int)>(dlsym(dlopenRos, "_Z9Runturtleii"));
        Moveturtle(1,1);
        dlclose(dlopenRos);
}

int main (int argc, char **argv) {

        rclcpp::init(argc, argv);

        std::filesystem::file_time_type filetime = std::filesystem::last_write_time("/home/jessewl/work/C-Programs/dynamic_ros/build/test/libdlopenRos.so");
        std::string initial_filetime = get_filetime(filetime);

        std::thread thrd1(lib_update, initial_filetime, filetime);
        std::thread thrd2(loading_rosnode);

        while(rclcpp::ok()) {
                if (file_update) {

                thrd2.detach();
                std::cout << "Update Success" << std::endl;
                file_update = false;
                break;
                }
        }
        
        std::thread thrd3(loading_rosnode);

        thrd1.join();
        thrd3.join();

        rclcpp::shutdown();

        return 0;
}
