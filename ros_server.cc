#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

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
                reply->set_msg_reply(request->msg_request());
                return Status::OK;
        } 
};

void RunServer() {
        std::string server_address("localhost:9515");
        TestRosService service;

        grpc::EnableDefaultHealthCheckService(true);
        grpc::reflection::InitProtoReflectionServerBuilderPlugin();
        
        ServerBuilder builder;
        builder.AddListeningPort(server_address, InsecureServerCredentials());
        builder.RegisterService(&service);

        std::unique_ptr<Server> server(builder.BuildAndStart());
        std::cout << "Server listening on " << server_address << std::endl;

        server->Wait();
}

int main(int argc, char** argv) {
        RunServer();
        
        return 0;
}
