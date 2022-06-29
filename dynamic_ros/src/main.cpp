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

void check_lib_update(std::string initial_ft, std::filesystem::file_time_type ft) {

        while (rclcpp::ok()) {

                bool dirc_update = if_dirc_update(initial_ft, ft);

                if (dirc_update) {
                        
                        ft = std::filesystem::last_write_time(Lib_PATH);
                        initial_ft = get_dirctime(ft);
                        lib_management();
                }
                sleep(1);
        }
}

void loading_rosnode() {
        char* lib_path = const_cast<char*>(Lib_container[Lib_container.size()-1].c_str());
        //std::cout << Lib_container[0] << std::endl;
        const auto dlopenRos = dlopen (lib_path, RTLD_NOW);
        auto Moveturtle = reinterpret_cast<void(*)(int, int)>(dlsym(dlopenRos, "_Z9Runturtleii"));
        Moveturtle(1,1);
        dlclose(dlopenRos);
}

int main (int argc, char **argv) {

        rclcpp::init(argc, argv);

        std::filesystem::file_time_type filetime = std::filesystem::last_write_time(Lib_PATH);
        std::string initial_filetime = get_dirctime(filetime);

        std::thread thrd2(loading_rosnode);
        std::thread thrd1(check_lib_update, initial_filetime, filetime);
        

        while(rclcpp::ok()) {
                if (roslib_update) {
                
                thrd2.join();
                std::cout << "Update Success" << std::endl;
                roslib_update = false;
                break;
                //std::thread thrd3(loading_rosnode);
                //thrd3.join();
                }
        }
        //thrd2.join();
        

        std::cout << "log" << std::endl; //False

        thrd1.join();
        

        rclcpp::shutdown();

        return 0;
}
