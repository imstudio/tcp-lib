
#include <chrono>
#include "netcore.h"
#include "utils/logger.h"

BEGIN_NAMESPACE(fnet)

NetCore::NetCore(){
}

NetCore::~NetCore(){
}

void NetCore::write_message(int64_t conn_id, TcpMessage& msg) {
  auto it = _conns.find(conn_id);
  if (it != _conns.end()){
    it->second->send(msg);
  }
}

void NetCore::close_conn(std::size_t conn_id){
  auto it = _conns.find(conn_id);
  FLOG(info) << "close conn:" << conn_id;
  if (it != _conns.end()) {
    FLOG(info) << "conn exist.";
    it->second->close(false);
    _conns.erase(it);
  }
}

void NetCore::start(int port){
  _server.reset(new TcpServer(port, _queue));
  _server->set_add_conn_func([this](ConnectionPtr conn){
      _conns[conn->get_conn_id()] = conn;
      conn->set_socket_close_callback(
          std::bind(&NetCore::close_conn, this, std::placeholders::_1));
    });
  _server->start();
  while(true) {
    TcpMessage msg;
    auto success = _queue.wait_dequeue_timed(msg, std::chrono::milliseconds(5000));
    if (!success && _stop)
      break;
    if (success){
      FLOG(info)<<"Recieve msg: ["<< std::string(msg.data(), msg.body_length())
                <<"] with owner:" << msg.get_owner();
      FLOG(info)<<"Length: "<<msg.body_length();
    }
  }
  _server->stop();
}

END_NAMESPACE
