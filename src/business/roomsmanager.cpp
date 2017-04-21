#include "roomsmanager.h"
#include "bizdata.h"
#include "bizdefine.h"
#include "utils/logger.h"
#include "pb/game_data.pb.h"


BEGIN_NAMESPACE(fgame)
using namespace fnet;
bool RoomsManager::do_process(fnet::TcpMessagePtr msg) {
  FLOG(info)<<"Recieve msg: ["<< std::string(msg->data(), msg->length())
            <<"] with owner:" << msg->get_owner();
  bool success = false;

  OutMsgBuffer buf;
  do {
    BizData biz_data;
    if (!biz_data.set_data(msg)) {
      FLOG(info) << "set biz data failed.";
      break;
    }

    auto itr = _rooms.find(biz_data.room_id);
    if (itr == _rooms.end()) {
      IRoomPtr room(new IRoom(_worker_service, biz_data.room_id));
      if (room->initialize(biz_data)) {
        _rooms[biz_data.room_id] = room;
        success = true;
      }
      break;
    }
    success = itr->second->process(biz_data, buf)
  } while(false);

  // prototype code.
  buf.set_msg_type(biz_data.msg_type);
  buf.set_msg_extra(biz_data.room_id);
  buf.msg_content = std::move(std::string(msg->data(), msg->length()));
  send_message(msg->get_owner(), buf);
  return success;
}

void RoomsManager::disconnect(std::size_t conn_id) {

}

void RoomsManager::tick() {

}

END_NAMESPACE
