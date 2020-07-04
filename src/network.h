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
  std::vector<IpV4Address> GetIpv4Addresses();
  std::vector<IpV6Address> GetIpv6Addresses();
  bool HasIpv6Address();
  std::string GetMacAddress();

private:
  int SetIpAddresses();
  void SetMacAddress();

  std::vector<IpV4Address> _ipv4_addresses;
  std::vector<IpV6Address> _ipv6_addresses;
  std::string _mac;
};

#endif /* NETWORK_H_ */