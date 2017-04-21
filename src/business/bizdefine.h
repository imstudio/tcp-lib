#ifndef BIZDEFINE_H
#define BIZDEFINE_H

#include <vector>

namespace fgame{
  enum MessageType {
    PB_HeartBeat = 0,
    PB_CreateRoom = 1,
    PB_EnterRoom = 2
  };

  struct RoomInfo{
    uint8_t game_type{0};
    uint8_t status{0};
    uint32_t real_id{0};
    uint32_t owner_id{0};
    std::vector<uint8_t> game_conf;
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
    std::size_t conn_id;
    std::string name{""};
    std::string avatar{""};

    explicit PlayerInfo(uint32_t id_, std::size_t conn_id_)
        :id(id_),conn_id(conn_id_) {}

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
