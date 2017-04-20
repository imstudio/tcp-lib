#include "gamebizimpl.h"
#include "bizdata.h"
#include "bizdefine.h"
#include "utils/logger.h"
#include "pb/main.pb.h"

BEGIN_NAMESPACE(fgame)
using namespace fnet;

bool GameBizImpl::do_process(fnet::TcpMessagePtr msg) {
  FLOG(info)<<"Recieve msg: ["<< std::string(msg->data(), msg->length())
            <<"] with owner:" << msg->get_owner();
  BizData biz_data;
  if (!biz_data.set_data(msg)) {
    FLOG(info) << "set biz data failed.";
    return false;
  }
  auto p_msg = get_gmsg_by_type(biz_data.msg_type);
  //bool success = p_msg->ParseFromArray(biz_data.pb_data(), biz_data.pb_size());
  OutMsgBuffer buf;
  buf.set_msg_type(biz_data.msg_type);
  buf.set_msg_extra(biz_data.room_id);
  buf.msg_content = std::move(std::string(msg->data(), msg->length()));
  send_message(msg->get_owner(), buf);
  return true;
}

void GameBizImpl::disconnect(std::size_t conn_id) {
}

GameBizImpl::GMessagePtr GameBizImpl::get_gmsg_by_type(uint8_t t) {
  return GMessagePtr(new tutorial::AddressBook);
}

bool GameBizImpl::create_room(int32_t uid, int32_t room_id) {
  auto itr = _rooms.find(room_id);
  if (itr != _rooms.end()) {
    // already create.
    return true;
  }
  IRoomPtr room(new IRoom(_worker_service));
  // get conf from redis
  // init room instance
  _rooms[room_id] = room;
  return true;
}

void GameBizImpl::tick() {
}


END_NAMESPACE
