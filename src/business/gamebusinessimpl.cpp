#include "gamebusinessimpl.h"
#include "utils/logger.h"

BEGIN_NAMESPACE(fgame)
using namespace fnet;

bool GameBusinessImpl::do_process(fnet::TcpMessagePtr msg) {
  FLOG(info)<<"Recieve msg: ["<< std::string(msg->data(), msg->body_length())
            <<"] with owner:" << msg->get_owner();
  FLOG(info)<<"Length: "<<msg->body_length();
  return true;
}

END_NAMESPACE
