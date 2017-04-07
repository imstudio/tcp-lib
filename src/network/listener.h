#ifndef LISTENER_H
#define LISTENER_H

#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/system/error_code.hpp>
#include <boost/bind/bind.hpp>

using namespace boost::asio;
using namespace std;


BEGIN_NAMESPACE(fnet)

class Listener{
  typedef Listener this_type;
  typedef ip::tcp::acceptor acceptor_type;
  typedef ip::tcp::endpoint endpoint_type;
  typedef ip::tcp::socket socket_type;
  typedef ip::address address_type;
  typedef shared_ptr<socket_type> sock_ptr;
public:
Listener():m_acceptor(m_io, endpoint_type(ip::tcp::v4(), 6315)){
    accept();
  }

  void run(){
    m_io.run();
  }

  void accept(){
    sock_ptr sock(new socket_type(m_io));
    m_acceptor.async_accept(*sock, boost::bind(&this_type::accept_hander, this, boost::asio::placeholders::error, sock));
  }

  void accept_hander(const boost::system::error_code &ec, sock_ptr sock) {
    if (ec) {
      return;
    }
    cout << "Client:";
    cout << sock->remote_endpoint().address()<<endl;
    sock->async_write_some(buffer("hello asio"), boost::bind(&this_type::write_handler, this, boost::asio::placeholders::error));
    accept();
  }

  void write_handler(const boost::system::error_code &ec) {
    cout << "send msg over" << endl;
  }
  ~Listener() = default;

private:
    io_service m_io;
    acceptor_type m_acceptor;
};

END_NAMESPACE
#endif
