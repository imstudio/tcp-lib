#ifndef BIZDATA_H
#define BIZDATA_H

#include <vector>
#include "bizdefine.h"
#include "utils/common_define.h"
#include "network/tcpmessage.h"
#include "pb/game_data.pb.h"

BEGIN_NAMESPACE(fgame)

#define PACKET_INFO_LEN (MSG_HEADER_LEN - fnet::TcpMessage::header_length)

enum PbMsgType{
  Enum_EnterRoom = 1
};

struct BizOutData{
  std::vector<std::size_t> users;
  fnet::OutMsgBuffer data;
};

struct BizData{
  uint8_t msg_type{0};
  t_room room_id{0};
  t_conn conn_id{0};
  fnet::TcpMessagePtr p_msg;

  inline bool set_data(fnet::TcpMessagePtr msg) {
    auto len = msg->length();
    if (len < PACKET_INFO_LEN) {
      return false;
    }
    auto ch_ary = msg->data();
    msg_type = static_cast<uint8_t>(ch_ary[0]);
    room_id |= ch_ary[1] << 24;
    room_id |= ch_ary[2] << 16;
    room_id |= ch_ary[3] << 8;
    room_id |= ch_ary[4];
    p_msg = msg;
    conn_id = msg->get_owner();
    return true;
  }

  const char* pb_data() const {
    return p_msg->data() + PACKET_INFO_LEN;
  }

  std::size_t pb_size() const {
    return p_msg->length() - PACKET_INFO_LEN;
  }
};

typedef std::shared_ptr<google::protobuf::Message> PbMessagePtr;

class PbMessageFactory{
public:
  static PbMessagePtr get_message(uint8_t msg_type) {
    if (msg_type == Enum_EnterRoom) {
      return PbMessagePtr(new EnterRoomData());
    }
    return PbMessagePtr();
  }
};

struct GameData{
  std::vector<uint8_t> target_user;
  PbMessagePtr pb_message;
};

END_NAMESPACE
#endif
