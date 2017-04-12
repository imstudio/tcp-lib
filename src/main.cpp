#include <iostream>
using namespace std;

#include "utils/logger.h"
#include "network/netcore.h"

int main(){
  init_fnet_logger("../logs/ty");
  fnet::NetCore nc;
  nc.start(7777);
  return 0;
}
