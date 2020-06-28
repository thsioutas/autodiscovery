#ifndef NETWORK_H_
#define NETWORK_H_

#include <ifaddrs.h>
#include <vector>

class NetworkInterface {
 public:
  NetworkInterface(std::string name);
  std::string _name;

 private:
  std::vector<std::string> _ipv4_addresses;
};

#endif /* NETWORK_H_ */