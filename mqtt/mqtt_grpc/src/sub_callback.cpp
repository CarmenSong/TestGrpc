#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <string>

#include "grpcpp/grpcpp.h"
#include "Message.pb.h"
#include "Message.grpc.pb.h"
#include "mosquitto.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using mesg::Message;
using mesg::Transfer;
using mesg::MessageReply;

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
        //std::cout << "Got Message: " << message.DebugString();
        std::string result = message.body();
        std::cout << result << std::endl;

    }


class MessageHandler {
public:
    MessageHandler(std::shared_ptr<Channel> channel)
    : stub_(Transfer::NewStub(channel)) {}

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
    std::unique_ptr<Transfer::Stub> stub_;
    std::string _host;
    int id = 12;
    int rc;
    std::string mesg_pub_topic = "MessageHandler/publish";
};

 
int main(int argc, char **argv) {

    std::string target_str;
    std::string arg_str("--target");
    if (argc > 1) {
        std::string arg_val = argv[1];
        size_t start_pos = arg_val.find(arg_str);
        if (start_pos != std::string::npos) {
            start_pos += arg_str.size();
            if (arg_val[start_pos] == '=') {
                 target_str = arg_val.substr(start_pos + 1);
                } else {
                    std::cout << "The only correct argument syntax is --target="
                    << std::endl;
                    return 0;
                }
    } else {
            std::cout << "The only acceptable argument is --target=" << std::endl;
            return 0;
            }
        } else {
            target_str = "localhost:9515";
            }

    MessageHandler handle(
        grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials())
    );
    handle.init();
    handle.call_back();

    // Optional:  Delete all global objects allocated by libprotobuf.
    google::protobuf::ShutdownProtobufLibrary();
 
    return 0;
}