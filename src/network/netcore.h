#ifndef NETCORE_H
#define NETCORE_H

#include <unordered_map>
#include <atomic>
#include "utils/common_define.h"
#include "tcpserver.h"

BEGIN_NAMESPACE(fnet)

#define MAX_MSG_QUEUE_SIZE 512

class NetCore{
public:
  NetCore();
  ~NetCore();

  void write_message(int64_t conn_id, TcpMessage& msg);

  void add_conn(ConnectionPtr conn);

  void close_conn(std::size_t conn_id);

  void start(int port);

  void stop() {_stop = true;}

private:
  std::unordered_map<int64_t, ConnectionPtr> _conns;
  BlockMessageQueue _queue{MAX_MSG_QUEUE_SIZE};
  TcpServerPtr _server;
  bool _stop{false};
};

END_NAMESPACE
#endif
