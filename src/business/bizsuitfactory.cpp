#include "network/bizsuitfactory.h"
#include "gamebusinessimpl.h"

namespace fnet{
std::shared_ptr<IBizSuit> get_biz_suit(io_service& service, IBizSuit::SendMsgFuncType f) {
  return std::shared_ptr<IBizSuit>(new fgame::GameBusinessImpl(service, f));
}
};
