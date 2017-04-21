#include "iroom.h"

BEGIN_NAMESPACE(fgame)

IRoom::IRoom(boost::asio::io_service& s):_worker_service(s){
}

IRoom::~IRoom(){
}

bool IRoom::process(const BizData& biz_data, fnet::OutMsgBuffer& out) {
  return true;
}

bool IRoom::initialize() {
  return true;
}

bool IRoom::disconnect_event(int64_t conn_id) {
  return true;
}

bool IRoom::tick(){

}

bool IRoom::user_in(PlayerInfo&& uinfo) {
  return true;
}

END_NAMESPACE
