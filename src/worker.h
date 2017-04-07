#ifndef WORKER_H
#define WORKER_H

#include "utils/datapacket.h"
using namespace utils;

BEGIN_NAMESPACE(lsrv)

class Worker{
public:
  Worker();
  virtual ~Worker();

  bool doWork(DataPacketPtr data) = 0;
};

END_NAMESPACE
#endif
