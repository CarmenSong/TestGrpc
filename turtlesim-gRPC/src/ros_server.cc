#include <iostream>
#include <memory>
#include <string>
#include <thread>


#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"

#include "RunTurtle.h"
#include "TestRos.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using TestRos::Turtlesim;
using TestRos::TurtleRequest;
using TestRos::TurtleReply;

class TestRosService : public Turtlesim::Service {

        Status MoveTurtle(ServerContext* context, const TurtleRequest* request,
        TurtleReply* reply) {

                auto node = rclcpp::Node::make_shared("turtle_controller");
                auto publisher = node->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel",1);

                auto msg = std::make_shared<geometry_msgs::msg::Twist>();

                while (request->msg_request() != "false") {
                        msg->linear.x = stof(request->msg_request());
                        msg->angular.z = stof(request->msg_request());
                        publisher->publish(*msg);

                        reply->set_msg_reply("Connection Success!");
                        //std::cout << "Connection Success! " << request->msg_request() << std::endl;
                //std::cout << request->msg_request() << std::endl;

                return Status::OK;
        } 
};
};

void RunServer() {

        std::string server_address("localhost:9515");
        TestRosService service;      

        grpc::EnableDefaultHealthCheckService(true);
        grpc::reflection::InitProtoReflectionServerBuilderPlugin();
        
        ServerBuilder builder;
        builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
        builder.RegisterService(&service);

        std::unique_ptr<Server> server(builder.BuildAndStart());
        std::cout << "Server listening on " << server_address << std::endl;

        server->Wait();
}

int main(int argc, char** argv) {

        rclcpp::init(argc, argv);

        //RunTurtle();
        //rclcpp::shutdown();
        std::thread thr2(RunServer);
        thr2.join();
        rclcpp::shutdown();
        

        return 0;
}
