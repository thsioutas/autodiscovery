#include <iostream>

#include "autodiscovery.h"
#include "network.h"

int main() {
  NetworkInterface interface = NetworkInterface("enp0s8");
  Autodiscovery ad = Autodiscovery(interface);
  ad.Discover();
  return 0;
}