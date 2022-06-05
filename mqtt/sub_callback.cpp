#include <iostream>
#include <fstream>
#include <vector>
#include "Message.pb.h"
#include "mosquitto.h"
 
using namespace std;

namespace mesg {

class MessageHandler {
public:
    MessageHandler(const std::string& host, const int port)
        : _host(host)
        , _port(port)
    {}

    ~MessageHandler() {
        if (_connected) {
            mosquitto_destroy(_mosq);
        }
    }
    void init() {
        // init publish struct
        _mosq = mosquitto_new(nullptr, true, nullptr);
        if (mosquitto_connect(_mosq, _host.c_str(), _port, _keep_alive) != MOSQ_ERR_SUCCESS) {
            std::cerr << "connect error!!" << std::endl;
        } else {
            _connected = true;
        }
    }


private:
    struct mosquitto* _mosq = nullptr;
    std::string _host;
    int _port;
    int _keep_alive = 60;
    bool _connected = false;
    std::string _mesg_pub_topic = "MessageHandler/publish";
};
}
 
int main(int argc, char *argv[]) {

    GOOGLE_PROTOBUF_VERIFY_VERSION;

    mesg::MessageHandler handle("mqtt.eclipse.org", 1883);
    handle.init();

    // Optional:  Delete all global objects allocated by libprotobuf.
    google::protobuf::ShutdownProtobufLibrary();
 
    return 0;
}