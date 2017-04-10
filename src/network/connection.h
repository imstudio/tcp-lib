#ifndef CONNECTION_H
#define CONNECTION_H


#include <cstdlib>
#include <deque>
#include <iostream>
#include <atomic>
#include <chrono>
#include "utils/common_define.h"
#include "utils/logger.h"
#include "asio_define.h"
#include "tcpmessage.h"
#include "readerwriterqueue.h"

BEGIN_NAMESPACE(fnet)

#define MAX_ALIVE_TIME 15

using namespace std;

typedef moodycamel::BlockingReaderWriterQueue<TcpMessage> BlockMessageQueue;

class Connection:public enable_shared_from_this<Connection>{
public:
  typedef std::function<void(int64_t)> ConnCloseFuncType;

Connection(
    std::size_t conn_id,
    io_service &service,
    BlockMessageQueue &queue,
    sock_ptr socket):
  _id(conn_id), _service(service), _r_msg_queue(queue), _socket(socket)
  {}

  void start() {
    _timer.reset(new deadline_timer(_service, asio_secs(MAX_ALIVE_TIME)));
    do_check_timeout();
    do_read_header();
  }

  void close(bool do_func) {
    // TODO: may be add a lock here?
    if (_socket->is_open()){
      boost::system::error_code ignored_err;
      _socket->shutdown(socket_base::shutdown_both, ignored_err);
      _socket->close(ignored_err);
    }
    if (do_func)
      _close_func(_id);
  }

  void set_socket_close_callback(ConnCloseFuncType func) {
    _close_func = func;
  }

  std::size_t get_conn_id(){
    return _id;
  }

  void do_check_timeout(){
    auto self(shared_from_this());
    _timer->async_wait(
        [this, self](const boost::system::error_code &ec) {
          if (!ec) {
            auto now = std::chrono::system_clock::now();
            if (std::chrono::duration_cast<std::chrono::seconds>(
                    now - _last_hb).count() > MAX_ALIVE_TIME)
            {
              FLOG(info) << "connection no heartbeat anymore, force leave.";
              close(true);
            }else{
              do_check_timeout();
            }
          }else {
            FLOG(info) << "do check timeout failed:" << ec;
            close(true);
          }
        });
  }

  void do_read_header() {
    auto self(shared_from_this());
    async_read(*_socket, buffer(_header_buffer, TcpMessage::header_length),
               [this, self](boost::system::error_code ec, std::size_t){
                 if (!ec)
                 {
                   auto body_length = decode_header();
                   if (body_length > TcpMessage::max_body_length) {
                     FLOG(info) << "invalid body_length:" << body_length;
                     close(true);
                   }else {
                     do_read_body(body_length);
                   }
                 }else{
                   FLOG(info) << "do read header failed:" << ec;
                   close(true);
                 }
               });
  }

  void do_read_body(std::size_t body_length){
    if (body_length == 0){
      _last_hb = std::chrono::system_clock::now();
      do_read_header();
      return;
    }
    auto self(shared_from_this());
    _msg.ensure(body_length);
    _msg.set_owner(_id);
    async_read(*_socket, buffer(_msg.data(), body_length),
               [this, self](boost::system::error_code ec, std::size_t){
                 if (!ec){
                   // debug code
                   FLOG(debug) << "Receive a msg from socket.";
                   if (!_r_msg_queue.try_enqueue(std::move(_msg))){
                     FLOG(info) << "queue is full!!!!!";
                   }
                   _last_hb = std::chrono::system_clock::now();
                   //process msg, or push data to queue
                   do_read_header();
                 } else{
                   FLOG(info) << "do read body failed." << ec;
                   close(true);
                 }
               });
  }

  std::size_t decode_header(){
    _header_buffer[TcpMessage::header_length] = '\0';
    return std::atoi(_header_buffer);
  }

  void do_write(){
    auto self(shared_from_this());
    _on_write = true;
    async_write(*_socket, buffer(_w_msg.front().data(), _w_msg.front().length()),
                [this, self](boost::system::error_code ec, std::size_t){
                  if (!ec){
                    _w_msg.pop_front();
                    if (!_w_msg.empty()){
                      do_write();
                    } else {
                      _on_write = false;
                    }
                  }else{
                    FLOG(info)<<"do write message failed:"<<ec;
                    close(true);
                  }
                });
  }

  void send(TcpMessage& msg) {
    // notice: Run in multiple thread.
    _w_msg.push_back(std::move(msg));
    if (!_on_write) {
      do_write();
    }
  }

private:
  std::size_t _id;
  io_service &_service;
  BlockMessageQueue &_r_msg_queue;
  sock_ptr _socket;
  TcpMessage _msg;
  deque<TcpMessage> _w_msg;
  std::chrono::time_point<std::chrono::system_clock> _last_hb;
  deadline_timer_ptr _timer;
  ConnCloseFuncType _close_func;
  char _header_buffer[TcpMessage::header_length + 1];
  bool _on_write{false};
};

DF_SHARED_PTR(Connection);

END_NAMESPACE
#endif
