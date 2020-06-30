#include <iostream>

#include <arpa/inet.h>
#include <cstring>
#include <ifaddrs.h>

#include "network.h"

NetworkInterface::NetworkInterface(std::string name) {
  _name = name;
  std::cout << "Construct network interface: " << _name << std::endl;
  if (SetIpAddresses() != 0) {
    std::cerr << "Error during IpAddresses retrieval" << std::endl;
  }
}

int NetworkInterface::SetIpAddresses() {
  struct ifaddrs *interfaces, *ifa;
  if (getifaddrs(&interfaces) != 0) {
    std::cerr << "Unalbe to retrieve network interfaces from the system!"
              << std::endl;
    return 1;
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
          std::cerr << "Unalbe to retrieve IPv4 address from the interface"
                    << _name << std::endl;
        } else {
          address.addr = addr;
          in = (struct sockaddr_in *)ifa->ifa_netmask;
          if (inet_ntop(AF_INET, &in->sin_addr, addr, INET_ADDRSTRLEN) ==
              NULL) {
            std::cerr << "Unalbe to retrieve IPv4 address from the interface"
                      << _name << std::endl;
          } else {
            address.netmask = addr;
            _ipv4_addresses.push_back(address);
          }
        }
      } else if (ifa->ifa_addr->sa_family == AF_INET6) {
        struct sockaddr_in6 *in = (struct sockaddr_in6 *)ifa->ifa_addr;
        struct IpV6Address address;
        if (inet_ntop(AF_INET6, &in->sin6_addr, addr, INET6_ADDRSTRLEN) ==
            NULL) {
          std::cerr << "Unalbe to retrieve IPv6 address from the interface!"
                    << _name << std::endl;
        } else {
          address.addr = addr;
          _ipv6_addresses.push_back(address);
        }
      } else {
        continue;
      }
    }
  }
  freeifaddrs(interfaces);
  return 0;
}

std::vector<IpV4Address> NetworkInterface::GetIpv4Addresses() {
  return _ipv4_addresses;
}

std::vector<IpV6Address> NetworkInterface::GetIpv6Addresses() {
  return _ipv6_addresses;
}

bool NetworkInterface::HasIpv6Address() {
  if (_ipv6_addresses.size() > 0) {
    return true;
  }
  return false;
}