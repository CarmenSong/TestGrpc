#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"

#include "TestRos.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using TestRos::Turtlesim;
using TestRos::TurtleRequest;
using TestRos::TurtleReply;

class TestRosService : public Turtlesim::Service {
        Status MoveTurtle(const TurtleRequest* request,
        TurtleReply* reply) {
                
                std::cout << request->msg_request() << std::endl;
                reply->set_msg_reply(110);

                auto node = rclcpp::Node::make_shared("turtle_controller");
                auto publiser = node->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 1);

                rclcpp::WallRate loop(1);
                while (rclcpp::ok()) {
                        auto msg = geometry_msgs::msg::Twist();
                        msg.linear.x = request->msg_request();
                        msg.linear.y = request->msg_request();
                        publiser->publish(msg);
                        loop.sleep();
                }

                rclcpp::shutdown();

                return Status::OK;
        } 


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
        RunServer();
        
        return 0;
}
