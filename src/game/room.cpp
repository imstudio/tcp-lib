#include "room.h"
#include "utils/logger.h"
#include "pb/game_data.pb.h"

BEGIN_NAMESPACE(fgame)

Room::Room(boost::asio::io_service& s, t_room room_id)
:_worker_service(s),_room_id(room_id)
{
}

Room::~Room(){
}

bool Room::process(const BizData& biz_data, BizOutData& out) {
  if (biz_data.msg_type == Enum_EnterRoom) {
    return do_enter_room(biz_data, out);
  }
  return true;
}

bool Room::initialize(const BizData& biz_data) {
  if (biz_data.msg_type != Enum_EnterRoom) {
    FLOG(info) << "invalid msg type:" << biz_data.msg_type;
    return false;
  }
  RoomInfo room;
  if (!get_room_info(biz_data.room_id, room)) {
    FLOG(info) << "get room [" << biz_data.room_id << "] info failed.";
    return false;
  }
  EnterRoomData data;
  if (!data.ParseFromArray(biz_data.pb_data(), biz_data.pb_size())) {
    return false;
  }
  // if (p_msg->userId() != room.owner_id) {
  //   FLOG(info) << "user [" << p_msg->userId() << "] no right.";
  //   return false;
  // }
  // first slot always is owner.
  _slots[0].reset_user(data.userid());
  return true;
}

bool Room::do_disconnect_event(t_conn conn_id, BizOutData& out) {
  for(t_slot i = 0; i < MaxPlayerCount; ++i) {
    if (_slots[i].conn_id == conn_id) {
      if (i)
        // broadcast msg
        _slots[i].force_leave();
      else
        _slots[i].hold();
      return true;
    }
  }
  return false;
}

void Room::set_gameover_func(Room::GameOverFuncType process_func) {
  _gameover_func = process_func;
}


bool Room::tick(){
  ++_timer;
  switch (_cur_st) {
    case RoomStatus::InitialWait:
    case RoomStatus::MidfieldWait:{
      for (t_slot i = 1; i < MaxPlayerCount; ++i) {
        auto& slot = _slots[i];
        ++slot.timer;
        if (!slot.is_ready and slot.timer > MaxWaitReadytime) {
          // broadcast msg
          slot.force_leave();
        }
      }
      break;
    }
    case RoomStatus::Gaming:{
      BizOutData out;
      if (_game_inst.get() && !_game_inst->tick(out)) {
        FLOG(info) << "tick tick failed.";
      }
    }
  }
}

bool Room::gameover(){
  return _b_gameover;
}

bool Room::get_room_info(t_room room_id, RoomInfo& conf) {
  return true;
}

bool Room::get_user_info(t_user user_id, PlayerInfo& player) {
  return true;
}

bool Room::do_enter_room(const BizData& biz_data, BizOutData& out) {
  EnterRoomData data;
  if (!data.ParseFromArray(biz_data.pb_data(), biz_data.pb_size())) {
    return false;
  }
  t_slot slot_id;
  if (!locate_slot(data.userid(), slot_id)) {
    return false;
  }
  _slots[slot_id].reset_user(data.userid(), biz_data.conn_id);
  return true;
}

bool Room::locate_slot(t_user uid, t_slot& slot_id) {
  t_slot i = 0;
  for (; i < MaxPlayerCount; ++i) {
    if (_slots[i].user_id == uid) {
      slot_id = i;
      return true;
    }
  }
  return false;
}

void Room::fix_out_data(BizOutData& data) {
  for(auto& item: data.users) {
    item = _slots[static_cast<t_slot>(item & 0x0F)].conn_id;
  }
  data.data.set_msg_extra(_room_id);
}

#undef PARSE_PB_MESSAGE

END_NAMESPACE
