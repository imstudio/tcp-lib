#ifndef GAMESLOT_H
#define GAMESLOT_H

#include "utils/common_define.h"

BEGIN_NAMESPACE(fgame)

struct GameSlot{
  uint32_t user_id{0};
  bool is_locked{0};
  bool is_ready{0};
  uint8_t game_status{0};
};

END_NAMESPACE
#endif
