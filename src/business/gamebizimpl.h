#ifndef GAMEBIZIMPL_H
#define GAMEBIZIMPL_H

#include "network/ibizsuit.h"
#include "iroom.h"
#include <google/protobuf/message.h>
#include <map>

BEGIN_NAMESPACE(fgame)

class GameBizImpl:public fnet::IBizSuit{
public:
  typedef std::shared_ptr<google::protobuf::Message> GMessagePtr;
  explicit GameBizImpl(boost::asio::io_service& s, SendMsgFuncType f)
      :fnet::IBizSuit(s, f) {}

  virtual bool do_process(fnet::TcpMessagePtr msg);
  virtual void disconnect(std::size_t conn_id);
  virtual void tick();

private:
  bool parse_packet(fnet::TcpMessage &msg);
  bool create_room(int32_t uid, int32_t room_id);

  static GMessagePtr get_gmsg_by_type(uint8_t t);
  std::map<int32_t, IRoomPtr> _rooms;
};

END_NAMESPACE
#endif