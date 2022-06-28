#include <filesystem>
#include <chrono>
#include <filesystem>
#include <ctime>
#include <iomanip>
#include <sstream>

std::string get_filetime(std::filesystem::file_time_type tp)
{
        auto sec = std::chrono::duration_cast<std::chrono::seconds>(tp.time_since_epoch());
        std::time_t t = sec.count();
        const std::tm* lt = std::localtime(&t);
        const std::string get_localtime = static_cast<std::ostringstream&&>(std::ostringstream() << std::put_time(lt, "%c")).str();

        return get_localtime;
        
}

bool if_lib_update(std::string initial_filetime, std::filesystem::file_time_type filetime)
{


        filetime = std::filesystem::last_write_time("/home/jessewl/work/C-Programs/dynamic_ros/build/test/libdlopenRos.so");
        std::string update_date = get_filetime(filetime);
        if (update_date != initial_filetime) return true;
        sleep(10);
        

        return false;

}
