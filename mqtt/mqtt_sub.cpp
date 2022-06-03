#include <stdio.h>
#include <stdlib.h>
#include <mosquitto.h>
#include <iostream>

void on_connect(struct mosquitto* mosq, void* obj, int rc) {
        if (rc) {
                std::cout << "Error" << std::endl;
                exit (-1);
        }
        mosquitto_subscribe(mosq, NULL, "test/t1", 0);
}

void on_message(struct mosquitto* mosq, void* obj, const struct mosquitto_message* msg) {
        std::cout << "New message: " << msg->topic <<  (char*) msg->payload << std::endl;
}

int main() {

        int rc, id = 12;
        
        mosquitto_lib_init();

        struct mosquitto* mosq;
        mosq = mosquitto_new("subscribe-test", true, &id);
        mosquitto_connect_callback_set(mosq, on_connect);
        mosquitto_message_callback_set(mosq, on_message);

        rc = mosquitto_connect(mosq, "localhost", 1883, 10);
        if (rc) {
                std::cout << "Connection Fail" << std::endl;
                return 0;
        }

        mosquitto_loop_start(mosq);
        getchar();
        mosquitto_loop_stop(mosq, true);

        mosquitto_disconnect(mosq);
        mosquitto_destroy(mosq);
        mosquitto_lib_cleanup();

        return 0;
}
