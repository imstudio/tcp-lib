#include "gamebusinessimpl.h"
#include "utils/logger.h"
#include "rapidjson/writer.h"

BEGIN_NAMESPACE(fgame)
using namespace fnet;

bool GameBusinessImpl::do_process(fnet::TcpMessagePtr msg) {
  FLOG(info)<<"Recieve msg: ["<< std::string(msg->data(), msg->body_length())
            <<"] with owner:" << msg->get_owner();
  FLOG(info)<<"Length: "<<msg->body_length();
  OutMsgBuffer buf;
  rapidjson::Writer<OutMsgBuffer> writer(buf);
  writer.StartObject();
  writer.Key("code", sizeof("code") - 1);
  writer.Int(200);
  writer.Key("data", sizeof("data") - 1);
  {
    writer.StartObject();
    writer.Key("msg", sizeof("msg") - 1);
    writer.String(msg->data(), msg->body_length());
    writer.EndObject();
  }
  writer.EndObject();
  send_message(msg->get_owner(), buf);
  return true;
}

void GameBusinessImpl::disconnect(std::size_t conn_id) {
  
}


END_NAMESPACE
