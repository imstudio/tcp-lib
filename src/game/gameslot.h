#ifndef GAMESLOT_H
#define GAMESLOT_H

#include "utils/common_define.h"

BEGIN_NAMESPACE(fgame)

struct GameSlot{
  uint32_t user_id{0};
  uint32_t points{0};
  bool is_locked{0};
  bool is_ready{0};
  uint8_t timer{0};
  uint8_t rounds_played{0};
  std::size_t conn_id{0};

  void reset_user(uint32_t uid, std::size_t cid=0) {
    user_id = uid;
    is_ready = false;
    timer = 0;
    conn_id = cid;
  }

  void force_leave(){
    conn_id = 0;
    if (!rounds_played){
      user_id = 0;
      is_ready = false;
      timer = 0;
    }
  }

  void hold(){
    conn_id = 0;
  }

  bool has_user() {
    return user_id > 0 && conn_id > 0;
  }
};

END_NAMESPACE
#endif
