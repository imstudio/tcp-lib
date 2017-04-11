#ifndef NET_DEFINE_H
#define NET_DEFINE_H

#include <boost/asio.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/system/error_code.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "rapidjson/stringbuffer.h"

namespace fnet{

  using namespace boost::asio;

  typedef rapidjson::StringBuffer OutMsgBuffer;

  typedef boost::posix_time::seconds asio_secs;
  typedef ip::tcp::acceptor acceptor_type;
  typedef ip::tcp::endpoint endpoint_type;
  typedef ip::tcp::socket socket_type;
  typedef ip::address address_type;
  typedef std::shared_ptr<socket_type> sock_ptr;
  typedef std::shared_ptr<deadline_timer> deadline_timer_ptr;
}
#endif
