#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <chrono>
#include <boost/bind/bind.hpp>
#include "utils/common_define.h"
#include "asio_define.h"
#include "connection.h"
#include "utils/logger.h"
#include "utils/globalstatistics.h"

BEGIN_NAMESPACE(fnet)

using namespace std;

class TcpServer{
  typedef TcpServer this_type;
public:
  typedef function<void(ConnectionPtr)> AddConnFuncType;
TcpServer(int port, BlockMessageQueue &queue):
  m_acceptor(m_io, endpoint_type(ip::tcp::v4(), port)), _msg_queue(queue)
  {}

  ~TcpServer(){
    stop();
  }

  void set_add_conn_func(AddConnFuncType func){
    _add_conn_func = func;
  }

  void accept(){
    sock_ptr sock(new socket_type(m_io_socket));
    m_acceptor.async_accept(
        *sock, [this, sock](const boost::system::error_code &ec){
          if (ec) {
            FLOG(info) << "accept failed:"<< ec;
          } else {
            ++_id_counter;
            FLOG(debug) << "new connection:" << _id_counter;
            auto new_conn = ConnectionPtr(
                new Connection(_id_counter, m_io_socket, _msg_queue, sock));
            _add_conn_func(new_conn);
            new_conn->start();
            accept();
          }
        });
  }

  void start(){
    assert(_add_conn_func);
    _accept_thread.reset(new thread([this](){
          accept();
          m_io.run();
        }));
    _socket_thread.reset(new thread([this](){
          _work_ptr.reset(new io_service::work(m_io_socket));
          m_io_socket.run();
        }));
    utils::GlobalStatistics::instance()->server_start_time
        = std::chrono::system_clock::now();
  }

  void stop(){
    _work_ptr.reset();
    m_io_socket.stop();
    m_io.stop();
    _socket_thread->join();
    _accept_thread->join();
  }


private:
  io_service m_io;
  io_service m_io_socket;
  shared_ptr<io_service::work> _work_ptr;
  acceptor_type m_acceptor;
  bool _stop;
  shared_ptr<thread> _socket_thread;
  shared_ptr<thread> _accept_thread;
  BlockMessageQueue& _msg_queue;
  AddConnFuncType _add_conn_func;
  std::size_t _id_counter{0};
};

DF_SHARED_PTR(TcpServer);

END_NAMESPACE
#endif
