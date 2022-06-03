//#include <stdio>
#include <mosquitto.h>
#include <string>
#include <iostream>

int main() {
        int rc;
        struct mosquitto* mosq;

        mosquitto_lib_init();

        mosq = mosquitto_new("publisher-test", true, NULL);

        rc = mosquitto_connect(mosq, "localhost", 1883, 60);
        if (rc!=0) {
                std::cout << "Cannot connect to broker " << rc <<  std::endl;
                mosquitto_destroy(mosq);
                return -1;
        }
        std::cout << "Broker Connectiong Success! \n" << std::endl;;
        
        
        std::string mqtt_msg;
        std::cin >> mqtt_msg;
        const void* msg_pub = mqtt_msg.c_str();
        mosquitto_publish(mosq, NULL, "test/t1", sizeof(mqtt_msg), msg_pub, 0, false);

        mosquitto_disconnect(mosq);
        mosquitto_destroy(mosq); // メモリ開放

        mosquitto_lib_cleanup();
        return 0;

}
