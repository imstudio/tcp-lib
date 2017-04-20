#ifndef NET_DEFINE_H
#define NET_DEFINE_H

#include <boost/asio.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/system/error_code.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#define MSG_HEADER_LEN 9

namespace fnet{

  struct OutMsgBuffer{
    char msg_header[MSG_HEADER_LEN];
    std::string msg_content;
    inline void set_msg_type(uint8_t t) {msg_header[4] = static_cast<char>(t);}
    inline void set_msg_extra(uint32_t ext) {
      msg_header[5] = static_cast<char>((ext >> 24) & 0x00FF);
      msg_header[6] = static_cast<char>((ext >> 16) & 0x00FF);
      msg_header[7] = static_cast<char>((ext >> 8) & 0x00FF);
      msg_header[8] = static_cast<char>(ext & 0xFF);
    }
    const char* get_head() {
      auto len = msg_content.size();
      msg_header[0] = static_cast<char>((len >> 24) & 0x00FF);
      msg_header[1] = static_cast<char>((len >> 16) & 0x00FF);
      msg_header[2] = static_cast<char>((len >> 8) & 0x00FF);
      msg_header[3] = static_cast<char>(len & 0xFF);
      //sprintf(msg_header, "%04d", static_cast<int32_t>(msg_content.size() & 0xFFFFFFFF));
      return msg_header;
    }
    inline std::size_t get_head_size() {
      return MSG_HEADER_LEN;
    }
    inline std::size_t get_body_size(){return msg_content.size();}
    inline const char *get_body(){return msg_content.data();}
  };

  using namespace boost::asio;

  // typedef rapidjson::StringBuffer OutMsgBuffer;
  typedef std::shared_ptr<OutMsgBuffer> OutMsgBufferPtr;

  typedef boost::posix_time::seconds asio_secs;
  typedef ip::tcp::acceptor acceptor_type;
  typedef ip::tcp::endpoint endpoint_type;
  typedef ip::tcp::socket socket_type;
  typedef ip::address address_type;
  typedef std::shared_ptr<socket_type> sock_ptr;
  typedef std::shared_ptr<deadline_timer> deadline_timer_ptr;
}
#endif
