#include <iostream>
#include <string>
#include <sstream>
#include <thread>

#include <grpc++/grpc++.h>

#include "./stack.grpc.pb.h"

#include "./config.h"


using std::cout;
using std::endl;
using std::cin;
using std::string;
using std::getline;
using std::ostringstream;
using std::thread;

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using StackService::StackServer;
using DogType::Dog;
using StackService::Empty;
using StackService::StackSizeResponse;
using StackService::StackBoolResponse;


class StackClient {
 private:
    std::unique_ptr<StackServer::Stub> stub_;
 public:
    StackClient(std::shared_ptr<Channel> channel)
    : stub_(StackServer::NewStub(channel))
    {}

    bool Push(Dog item) {
        ClientContext context;
        Empty reply;

        Status status = this->stub_->Push(&context, item, &reply);
        if (status.ok()) {
            return true;
        } else {
            std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
        }

        return false;
    }

    Dog* Pop() {
        ClientContext context;
        Empty none;
        Dog* item = new Dog();

        Status status = this->stub_->Pop(&context, none, item);

        if (status.ok()) {
            return item;
        } else {
            std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
        }
        return NULL;
    }

    StackSizeResponse* Size()
    {
        ClientContext context;
        Empty none;
        StackSizeResponse* size = new StackSizeResponse();
        Status status = this->stub_->Size(&context, none, size);

        if (status.ok()) {
            return size;
        } else {
            std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
            return NULL;
        }
    }

    StackBoolResponse* IsEmpty()
    {
        ClientContext context;
        Empty none;
        StackBoolResponse* response = new StackBoolResponse();

        Status status = this->stub_->IsEmpty(&context, none, response);
        if(status.ok()) {
            return response;
        } else {
            std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
            return NULL;
        }
    }
};

//
//int main(int argc, char* argv[]) {
//
//
//    cout << "gRPC Stack Server example" << endl;
//
//    string config_file = "/etc/vetulus/services/stack_client.conf";
//
//    if (argc > 1) {
//        config_file = argv[1];
//    }
//
//    StackConfigLoader config;
//    config.load(config_file);
//
//    ostringstream config_str;
//    config_str << config.addr << ":" << config.port;
//
//    for (int i = 0; i < 50; i++) {
//
//        thread* th = new thread([&] {
//            StackClient client(grpc::CreateChannel(
//                config_str.str(), grpc::InsecureChannelCredentials()));
//            Dog item;
//            ostringstream ostr;
//            ostr << "Item " << i;
//            item.set_name(ostr.str());
//            cout << "Push(" << item.name() << ")" << endl;
//            client.Push(item);
//        });
//
//        th->detach();
//    }
//
//    std::this_thread::sleep_for(std::chrono::seconds(1));
//
//    return EXIT_SUCCESS;
//}
