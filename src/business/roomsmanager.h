#ifndef ROOMSMANAGER_H
#define ROOMSMANAGER_H

#include <map>
#include "iroom.h"
#include "network/ibizsuit.h"

BEGIN_NAMESPACE(fgame)

class RoomsManager:public fnet::IBizSuit{
public:
  explicit RoomsManager(boost::asio::io_service& s, SendMsgFuncType f)
      :fnet::IBizSuit(s, f){}

  ~RoomsManager()=default;

  virtual bool do_process(fnet::TcpMessagePtr msg);
  virtual void disconnect(std::size_t conn_id);
  virtual void tick();

private:
  std::map<int32_t, IRoomPtr> _rooms;
};

END_NAMESPACE
#endif
