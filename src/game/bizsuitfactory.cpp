#include "network/bizsuitfactory.h"
#include "roomsmanager.h"

namespace fnet{
std::shared_ptr<IBizSuit> get_biz_suit(io_service& service, IBizSuit::SendMsgFuncType f) {
  return std::shared_ptr<IBizSuit>(new fgame::RoomsManager(service, f));
}
};
