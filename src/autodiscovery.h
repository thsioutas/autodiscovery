#ifndef AUTODISCOVERY_H_
#define AUTODISCOVERY_H_

#include "network.h"

class Autodiscovery {
 public:
  void Discover();

 private:
  NetworkInterface _network_interface;
};

#endif /* AUTODISCOVERY_H_ */