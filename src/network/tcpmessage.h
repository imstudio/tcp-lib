#ifndef TCPMESSAGE_H
#define TCPMESSAGE_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include "utils/logger.h"

BEGIN_NAMESPACE(fnet)
#define HEADER_FORMAT "%04d"

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
    return _body_length;
  }

  ~TcpMessage(){
    if(_data) free(_data);
  }

  TcpMessage(TcpMessage&& msg) {
    _data = msg._data;
    _body_length = msg._body_length;
    _msg_owner = msg._msg_owner;
    msg._data = nullptr;
    msg._body_length = 0;
  }

  TcpMessage& operator=(TcpMessage&& msg) {
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
  TcpMessage(const TcpMessage& other) = delete;
  TcpMessage& operator=(const TcpMessage& other) = delete;

  std::size_t _msg_owner{0};
  std::size_t _body_length{0};
  char *_data{nullptr};
};

DF_SHARED_PTR(TcpMessage);

END_NAMESPACE
#endif
