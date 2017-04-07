#include <iostream>
using namespace std;

#include "network/netcore.h"

int main(){
  fnet::NetCore nc;
  nc.start(7777);
  return 0;
}
