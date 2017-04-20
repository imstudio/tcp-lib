#ifndef IROOM_H
#define IROOM_H

#include "utils/common_define.h"
#include "network/net_define.h"
#include "bizdata.h"
#include "gameslot.h"
#include <set>
#include <map>

BEGIN_NAMESPACE(fgame)

#define MaxPlayerCount 4

enum class GameStatus: uint16_t{
  WaitUser,
  Running
};

class IRoom{
public:
  IRoom(boost::asio::io_service& s);
  virtual ~IRoom();

  virtual bool process(const BizData& biz_data, fnet::OutMsgBuffer& out);

  virtual bool initialize();

  virtual bool disconnect_event(int64_t conn_id);

  virtual bool tick();

private:
  bool user_in(int32_t user_id);
  void user_out(int32_t user_id);

  boost::asio::io_service& _worker_service;
  int32_t _room_id{0};
  int32_t _owner_id{0};
  int32_t _open_time{0};
  uint8_t _user_count{1};
  GameStatus _cur_st{GameStatus::WaitUser};
  GameSlot slots[MaxPlayerCount];
  std::map<int32_t, int32_t> _conn_2_user;
};

DF_SHARED_PTR(IRoom);

END_NAMESPACE
#endif
