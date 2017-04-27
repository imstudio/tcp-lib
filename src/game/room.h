#ifndef ROOM_H
#define ROOM_H

#include "utils/common_define.h"
#include "network/net_define.h"
#include "bizdata.h"
#include "bizdefine.h"
#include "gameslot.h"
#include "igamelogic.h"
#include <set>
#include <map>

BEGIN_NAMESPACE(fgame)

#define MaxPlayerCount 4
#define MaxWaitReadytime 20


enum class RoomStatus: uint8_t{
  InitialWait,
      Gaming,
      MidfieldWait,
      Settlement,
      Close
};

class Room{
public:
  typedef std::function<void()> GameOverFuncType;

  Room(boost::asio::io_service& s, t_room room_id);
  ~Room();

  bool process(const BizData& biz_data, BizOutData& out);
  bool initialize(const BizData& data);
  bool do_disconnect_event(t_conn conn_id, BizOutData& out);
  void set_gameover_func(GameOverFuncType process_func);
  bool tick();
  bool gameover();

private:
  bool get_room_info(t_room room_id, RoomInfo& conf);
  bool get_user_info(t_user user_id, PlayerInfo& player);
  bool do_enter_room(const BizData& msg, BizOutData& out);
  bool locate_slot(t_user uid, t_slot& slot_id);
  void fix_out_data(BizOutData& out);

  boost::asio::io_service& _worker_service;
  t_room _room_id{0};
  t_user _owner_id{0};
  uint32_t _open_time{0};
  uint16_t _timer{0};
  uint8_t _user_count{1};
  RoomStatus _cur_st{RoomStatus::InitialWait};
  GameSlot _slots[MaxPlayerCount];
  GameOverFuncType _gameover_func;
  bool _b_gameover{false};
  bool _game_stared{true};
  IGameLogicPtr _game_inst;
};

DF_SHARED_PTR(Room);

END_NAMESPACE
#endif
