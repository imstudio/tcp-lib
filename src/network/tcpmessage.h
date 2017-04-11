#ifndef TCPMESSAGE_H
#define TCPMESSAGE_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include "utils/logger.h"

BEGIN_NAMESPACE(fnet)

class TcpMessage{
public:
  enum {header_length = 4};
  enum {max_body_length = 4096};
  TcpMessage(){
  }

  void ensure(std::size_t size) {
    if (_data) free(_data);
    _data = new char[size];
    _body_length = size;
  }

  void ensure_body(std::size_t size) {
    if (_data) free(_data);
    _data = new char[size + header_length];
    _body_length = size;
  }

  void set_owner(std::size_t owner){
    _msg_owner = owner;
  }

  std::size_t get_owner() const {
    return _msg_owner;
  }

  void reset() {
    if(_data) free(_data);
    _data = new char[header_length + max_body_length];
    _body_length = 0;
  }

  const char * data() const {
    return _data;
  }

  char *data(){
    return _data;
  }

  std::size_t length() const {
    return header_length + _body_length;
  }

  const char * body() const {
    return _data + header_length;
  }

  char * body(){
    return _data + header_length;
  }

  std::size_t body_length() const {
    return _body_length;
  }

  void body_length(std::size_t new_length){
    _body_length = new_length;
    if (_body_length > max_body_length)
      _body_length = max_body_length;
  }

  void encode_header(){
    char header[header_length + 1] = "";
    std::sprintf(header, "%4d", static_cast<int>(_body_length));
    std::memcpy(_data, header, header_length);
  }

  ~TcpMessage(){
    if(_data) free(_data);
  }

  TcpMessage(TcpMessage&& msg) {
    FLOG(info) << "right assigenment in tcpmessage when construct.";
    _data = msg._data;
    _body_length = msg._body_length;
    _msg_owner = msg._msg_owner;
    msg._data = nullptr;
    msg._body_length = 0;
  }

  TcpMessage& operator=(TcpMessage&& msg) {
    FLOG(info) << "right assigenment in tcpmessage when transfer";
    if (this != &msg){
      _data = msg._data;
      _body_length = msg._body_length;
      _msg_owner = msg._msg_owner;
      msg._data = nullptr;
      msg._body_length = 0;
    }
    return *this;
  }

private:
  std::size_t _msg_owner{0};
  std::size_t _body_length{0};
  char *_data{nullptr};
};

DF_SHARED_PTR(TcpMessage);

END_NAMESPACE
#endif
