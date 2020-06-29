#include <iostream>

#include <arpa/inet.h>
#include <ifaddrs.h>
#include <cstring>

#include "network.h"

const int MAX_ADDR_SIZE = 46;

NetworkInterface::NetworkInterface(std::string name) {
  _name = name;
  std::cout << "Look for network interface: " << _name << std::endl;
  struct ifaddrs *interfaces, *ifa;
  if (getifaddrs(&interfaces) != 0) {
    std::cerr << "Unalbe to retrieve network interfaces from the system!"
              << std::endl;
    // TODO(thsioutas): Exit
  }
  for (ifa = interfaces; ifa != NULL; ifa = ifa->ifa_next) {
    if (ifa->ifa_addr == NULL) {
      continue;
    }
    std::string str(ifa->ifa_name);
    char addr[INET6_ADDRSTRLEN];
    if (str == _name) {
      if (ifa->ifa_addr->sa_family == AF_INET) {
        struct sockaddr_in *in = (struct sockaddr_in *)ifa->ifa_addr;
        struct IpV4Address address;
        if (inet_ntop(AF_INET, &in->sin_addr, addr, INET_ADDRSTRLEN) == NULL) {
          std::cerr << "Unalbe to retrieve IPv4 address from the interface!"
                    << _name << std::endl;
          // TODO(thsioutas): Exit
        }
        address.addr = addr;
        in = (struct sockaddr_in *)ifa->ifa_netmask;
        if (inet_ntop(AF_INET, &in->sin_addr, addr, INET_ADDRSTRLEN) == NULL) {
          std::cerr << "Unalbe to retrieve IPv4 address from the interface!"
                    << _name << std::endl;
          // TODO(thsioutas): Exit
        }
        address.netmask = addr;
        _ipv4_addresses.push_back(address);
      } else if (ifa->ifa_addr->sa_family == AF_INET6) {
        struct sockaddr_in6 *in = (struct sockaddr_in6 *)ifa->ifa_addr;
        struct IpV6Address address;
        if (inet_ntop(AF_INET6, &in->sin6_addr, addr, INET6_ADDRSTRLEN) ==
            NULL) {
          std::cerr << "Unalbe to retrieve IPv6 address from the interface!"
                    << _name << std::endl;
          // TODO(thsioutas): Exit
        }
        address.addr = addr;
        _ipv6_addresses.push_back(address);
      } else {
        continue;
      }
    }
  }
  freeifaddrs(interfaces);
}