#ifndef BIZDEFINE_H
#define BIZDEFINE_H

namespace fgame{
  enum MessageType {
    PB_HeartBeat = 0,
    PB_CreateRoom = 1,
    PB_EnterRoom = 2
  };

  struct NiuNiuGameConf{
    uint8_t card_cost{0};
    uint8_t round_count{0};
    uint8_t player_limit{0};
    uint8_t game_mode{0};
    uint8_t min_bet{0};
    uint32_t owner_id{0};
  };

  struct PlayerInfo{
    uint8_t status{0};
    int32_t card_storage{0};
    uint32_t id{0};
    std::string name{""};
    std::string avatar{""};

    PlayerInfo()=default;

    PlayerInfo(const PlayerInfo&& src) {
      card_storage = src.card_storage;
      id = src.id;
      name = std::move(src.name);
      avatar = std::move(src.avatar);
      status = src.status;
    }

    PlayerInfo& operator= (const PlayerInfo&& src) {
      card_storage = src.card_storage;
      id = src.id;
      name = std::move(src.name);
      avatar = std::move(src.avatar);
      status = src.status;
      return *this;
    }
  };
};

#endif
