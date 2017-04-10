#ifndef BIZSUITFACTORY_H
#define BIZSUITFACTORY_H

#include "utils/common_define.h"
#include "asio_define.h"
#include "ibizsuit.h"

BEGIN_NAMESPACE(fnet)

IBizSuitPtr get_biz_suit(io_service& service, IBizSuit::SendMsgFuncType f);

END_NAMESPACE
#endif
