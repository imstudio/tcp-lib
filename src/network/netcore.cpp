#include <chrono>
#include "netcore.h"
#include "utils/logger.h"
#include "bizsuitfactory.h"

BEGIN_NAMESPACE(fnet)

NetCore::NetCore(){
  _biz = get_biz_suit(_work_service, [this](int64_t conn_id, TcpMessagePtr msg){
      auto it = _conns.find(conn_id);
      if (it != _conns.end()){
        it->second->send(*msg);
      }
    });
}

NetCore::~NetCore(){
  _work_ptr.reset();
  _work_service.stop();
  for (int i = 0; i < WORK_THREADS_COUNT; ++i) {
    _work_threads[i]->join();
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
  start_work_loop();
  _server.reset(new TcpServer(port, _queue));
  _server->set_add_conn_func([this](ConnectionPtr conn){
      _conns[conn->get_conn_id()] = conn;
      conn->set_socket_close_callback(
          std::bind(&NetCore::close_conn, this, std::placeholders::_1));
    });
  _server->start();
  while(true) {
    TcpMessagePtr msg(new TcpMessage);
    auto success = _queue.wait_dequeue_timed(*msg, std::chrono::milliseconds(5000));
    if (!success && _stop)
      break;
    if (success){
      _biz->process(msg);
    }
  }
  _server->stop();
}

void NetCore::start_work_loop(){
  _work_ptr.reset(new io_service::work(_work_service));
  for (int i = 0; i < WORK_THREADS_COUNT; ++i){
    _work_threads.push_back(
        std::shared_ptr<std::thread>(
            new std::thread(boost::bind(&io_service::run, &_work_service))));
  }
}

END_NAMESPACE
