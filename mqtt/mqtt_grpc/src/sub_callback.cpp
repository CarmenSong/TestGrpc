#include <iostream>
#include <fstream>
#include <vector>
#include "Message.pb.h"
#include "mosquitto.h"
 
using namespace std;

namespace mesg {

static Message message;

void on_connect(struct mosquitto* mosq, void* obj, int rc) {
        if (rc) {
            std::cout << "Error" << std::endl;
            exit(-1);
        }
        mosquitto_subscribe(mosq, NULL, "MessageHandler/publish",0);
    }

void on_message(struct mosquitto* mosq, void* obj, const struct mosquitto_message* msg) {
        std::cout << "From Topic" << msg->topic << std::endl;
        message.ParseFromArray((const char*)msg->payload, msg->payloadlen);
        std::cout << "Got Message: " << message.DebugString();
    }

class MessageHandler {
public:

    void init() {
        // init publish struct
        mosq = mosquitto_new("subsrcibe-test", true, &id);
        mosquitto_connect_callback_set(mosq, on_connect);
        mosquitto_message_callback_set(mosq, on_message);

        rc = mosquitto_connect(mosq, "localhost", 1883, 60);
        if (rc) {
            std::cout << "Connection Fail" << std::endl;
            return;
        }
    }

    void call_back() {
    mosquitto_loop_start(mosq);
    while (true) {
        std::cout << "press and key to exit" << std::endl;
        char c = getchar();
        break;
    }
    mosquitto_loop_stop(mosq, true);
    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
    }

private:
    struct mosquitto* mosq;
    std::string _host;
    int id = 12;
    int rc;
    std::string mesg_pub_topic = "MessageHandler/publish";
};
}
 
int main(int argc, char *argv[]) {

    mesg::MessageHandler handle;
    handle.init();
    handle.call_back();

    // Optional:  Delete all global objects allocated by libprotobuf.
    google::protobuf::ShutdownProtobufLibrary();
 
    return 0;
}
