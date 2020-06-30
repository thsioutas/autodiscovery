#ifndef NETWORK_H_
#define NETWORK_H_

#include <ifaddrs.h>
#include <vector>

struct IpV4Address {
  std::string addr;
  std::string netmask;
};

struct IpV6Address {
  std::string addr;
  // std::string prefix;
};

class NetworkInterface {
public:
  NetworkInterface(std::string name);
  std::string _name;

private:
  int SetIpAddresses();

  std::vector<IpV4Address> _ipv4_addresses;
  std::vector<IpV6Address> _ipv6_addresses;
};

#endif /* NETWORK_H_ */