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

  BizOutData buf;
  BizData biz_data;
  do {
    if (!biz_data.set_data(msg)) {
      FLOG(info) << "set biz data failed.";
      break;
    }

    auto itr = _rooms.find(biz_data.room_id);
    if (itr == _rooms.end() or itr->second->gameover()) {
      auto room_id = biz_data.room_id;
      RoomPtr room(new Room(_worker_service, room_id));
      if (room->initialize(biz_data)) {
        room->set_gameover_func([this, room_id](){
            _rooms.erase(room_id);
          });
        _rooms[biz_data.room_id] = room;
        success = true;
      }
      break;
    }
    success = itr->second->process(biz_data, buf);
  } while(false);

  // prototype code.
  buf.data.set_msg_type(biz_data.msg_type);
  buf.data.set_msg_extra(biz_data.room_id);
  buf.data.msg_content = std::move(std::string(msg->data(), msg->length()));
  send_message(msg->get_owner(), buf.data);
  return success;
}

void RoomsManager::disconnect(std::size_t conn_id) {

}

void RoomsManager::tick() {

}

END_NAMESPACE
