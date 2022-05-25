#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#include "TestRos.grpc.pb.h"
#include "TestRos.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using TestRos::Turtlesim;
using TestRos::TurtleRequest;
using TestRos::TurtleReply;

class TestRosClient {
        public:
        TestRosClient(std::shared_ptr<Channel> channel)
        : stub_(Turtlesim::NewStub(channel)) {}

        std::string MoveTurtle(const std::string& user) {
                TurtleRequest request;
                request.set_msg_request(user);

                TurtleReply reply;

                ClientContext context;

                Status status = stub_->MoveTurtle(&context, request, &reply);

                if (status.ok()) {
                        return reply.msg_reply();
                } else {
                        std::cout << status.error_code() << ":" << status.error_message() 
                        << std::endl;
                        return "RPC failed";
                }
        }

        private:
        std::unique_ptr<Turtlesim::Stub> stub_;
};

int main(int argc, char** argv) {
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
        TestRosClient testros(
                grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials())
        );
        std::string user;
        while (user != "false") {
                std::getline(std::cin, user);
                if (user == "end") return 0;
                std::string reply = testros.MoveTurtle(user);
                std::cout << reply << std::endl;    
}
