
#ifndef MANAGER_PROTO_CLIENT_H_
#define MANAGER_PROTO_CLIENT_H_

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <grpc++/grpc++.h>

#include "./service.grpc.pb.h"

#include "./config.h"

using std::cout;
using std::cerr;
using std::endl;
using std::unique_ptr;
using std::string;
using std::ifstream;
using std::stringstream;

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using VetulusService::ProtoFile;
using VetulusService::Ack;
using VetulusService::Manager;
using VetulusService::MetaData;

using VetulusService::ADTService;
using VetulusService::ListOptions;
using VetulusService::ListResponse;

using VetulusService::ADTImpl;
using VetulusService::ListADTsOptions;
using VetulusService::ListADTsResponse;



class ProtoClient {
 private:
    std::unique_ptr<Manager::Stub> stub_;
 public:
    explicit ProtoClient(std::shared_ptr<Channel> channel)
    : stub_(Manager::NewStub(channel))
    {}

    bool Add(const ProtoFile* file)
    {
        ClientContext context;
        Ack ack;

        Status status = stub_->Add(&context, *file, &ack);

        if (status.ok() && ack.done()) {
            return true;
        } else {
            return false;
        }
    }

    bool Remove(const string protofile)
    {
        ClientContext context;
        Ack ack;
        MetaData meta;
        meta.set_name(protofile);

        Status status = stub_->Remove(&context, meta, &ack);

        if (status.ok() && ack.done()) {
            return true;
        }
        return false;
    }

    string ReadFileAsString(const string path)
    {
        ifstream proto_file(path.c_str());
        stringstream buffer;
        buffer << proto_file.rdbuf();
        proto_file.close();
        return buffer.str();
    }

    bool Start(const ADTService* adt)
    {
      ClientContext context;
      Ack ack;
      Status status = stub_->Start(&context, *adt, &ack);

      if (status.ok() && ack.done()) {
          return true;
      }
      return false;
    }

    bool Stop(const ADTService* adt)
    {
      ClientContext context;
      Ack ack;
      Status status = stub_->Stop(&context, *adt, &ack);

      if (status.ok() && ack.done()) {
          return true;
      }
      return false;
    }

    ListResponse ListServices (const ListOptions* opts)
    {
      ClientContext context;
      ListResponse response;
      Status status = stub_->ListServices(&context, *opts, &response);

      return response;
    }

    ListADTsResponse ListADTs (const ListADTsOptions* opts)
    {
      ClientContext context;
      ListADTsResponse response;
      Status status = stub_->ListADTs(&context, *opts, &response);

      return response;
    }
};

#endif // MANAGER_PROTO_CLIENT_H_
