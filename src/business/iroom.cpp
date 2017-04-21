#include "iroom.h"
#include "utils/logger.h"

BEGIN_NAMESPACE(fgame)

#define PARSE_PB_MESSAGE(src, dest)                             \
  auto dest = PbMessageFactory::get_message(src.msg_type);      \
  if (dest.get() == nullptr) {                                  \
    FLOG(info) << "Invalid msg.";                               \
    return false;                                               \
  }

IRoom::IRoom(boost::asio::io_service& s, uint32_t room_id)
:_worker_service(s),_room_id(room_id)
{
}

IRoom::~IRoom(){
}

bool IRoom::process(const BizData& biz_data, fnet::OutMsgBuffer& out) {
  PARSE_PB_MESSAGE(biz_data, p_msg);
  return true;
}

bool IRoom::initialize(const BizData& biz_data) {
  if (biz_data.msg_type != Enum_EnterRoom) {
    FLOG(info) << "invalid msg type:" << biz_data.msg_type;
    return false;
  }
  RoomInfo room;
  if (!get_room_info(biz_data.room_id, room)) {
    FLOG(info) << "get room [" << biz_data.room_id << "] info failed.";
    return false;
  }
  PARSE_PB_MESSAGE(biz_data, p_msg);
  if (p_msg->userId() != room.owner_id) {
    FLOG(info) << "user [" << p_msg->userId() << "] no right.";
    return false;
  }
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

bool IRoom::get_room_info(uint32_t room_id, RoomInfo& conf) {
  return true;
}

bool IRoom::get_user_info(uint32_t user_id, PlayerInfo& player) {
  return true;
}

#undef PARSE_PB_MESSAGE


END_NAMESPACE
