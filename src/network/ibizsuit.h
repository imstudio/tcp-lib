#ifndef IBIZSUIT_H
#define IBIZSUIT_H

#include "net_define.h"
#include "utils/common_define.h"
#include "rapidjson/stringbuffer.h"
#include "tcpmessage.h"

BEGIN_NAMESPACE(fnet)

class IBizSuit{
public:

  typedef std::function<void(int64_t, OutMsgBuffer&)> SendMsgFuncType;

  explicit IBizSuit(io_service& s, SendMsgFuncType f)
      :_worker_service(s), _send_func(f){}

  virtual ~IBizSuit() {};

  virtual bool do_process(TcpMessagePtr msg) = 0;

  void send_message(std::size_t conn_id, OutMsgBuffer& buf) {
    _send_func(conn_id, buf);
  }

  void process(TcpMessagePtr msg) {
    _worker_service.post(std::bind(&IBizSuit::do_process, this, msg));
  }

  virtual void disconnect(std::size_t conn_id) = 0;

private:
  io_service& _worker_service;
  SendMsgFuncType _send_func;
};

DF_SHARED_PTR(IBizSuit);


END_NAMESPACE
#endif
