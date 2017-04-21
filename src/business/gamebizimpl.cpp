#include "gamebizimpl.h"
#include "bizdata.h"
#include "bizdefine.h"
#include "utils/logger.h"
#include "pb/game_data.pb.h"

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

void GameBizImpl::tick() {
}

GameBizImpl::GMessagePtr GameBizImpl::get_gmsg_by_type(uint8_t t) {
  return GMessagePtr(new EnterRoomData());
}

bool GameBizImpl::enter_room(uint32_t uid, uint32_t room_id) {
  PlayerInfo player;
  if (!get_user_info(uid, player)) {
    FLOG(warning) << "user[" << uid << "] not exist.";
    return false;
  }

  auto itr = _rooms.find(room_id);
  if (itr != _rooms.end()) {
    return itr->second->user_in(std::move(player));
  }
  NiuNiuGameConf conf;
  if (!get_room_conf(room_id, conf)){
    FLOG(warning) << "get room[" << room_id << "] conf failed.";
    return false;
  }
  if (conf.owner_id != uid) {
    FLOG(warning) << "room[" << room_id << "] not belong user[" << uid << ']';
    return false;
  }
  IRoomPtr room(new IRoom(_worker_service));
  // get conf from redis
  // init room instance
  _rooms[room_id] = room;
  return room->user_in(std::move(player));
}

bool GameBizImpl::get_room_conf(uint32_t room_id, NiuNiuGameConf& conf) {
    conf.card_cost = 1;
    conf.owner_id = 110;
    conf.round_count = 10;
    conf.player_limit = 4;
    conf.game_mode = 3;
    conf.min_bet = 1;
    return true;
}

bool GameBizImpl::get_user_info(uint32_t user_id, PlayerInfo& player) {
  player.id = user_id;
  player.name = "afanti";
  player.avatar = "http://www.baidu.com";
  player.card_storage = 100;
  return true;
}


END_NAMESPACE
