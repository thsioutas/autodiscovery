#include <arpa/inet.h>
#include <cstring>
#include <ifaddrs.h>
#include <iostream>
#include <net/if.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "network.h"

NetworkInterface::NetworkInterface(std::string name) {
  _name = name;
  std::cout << "Construct network interface: " << _name << std::endl;
  if (SetIpAddresses() != 0) {
    std::cerr << "Error during IpAddresses retrieval" << std::endl;
  }
  SetMacAddress();
}

int NetworkInterface::SetIpAddresses() {
  struct ifaddrs *interfaces, *ifa;
  if (getifaddrs(&interfaces) != 0) {
    std::cerr << "Unalbe to retrieve network interfaces from the system!"
              << std::endl;
    return 1;
  }
  int sd = socket(PF_INET, SOCK_DGRAM, 0);
  if (sd < 0) {
    /// free memory allocated by getifaddrs
    freeifaddrs(interfaces);
    return 1;
  }
  for (ifa = interfaces; ifa != NULL; ifa = ifa->ifa_next) {
    if (ifa->ifa_addr == NULL) {
      continue;
    }
    std::string str(ifa->ifa_name);
    if (str == _name) {
      if (ifa->ifa_addr->sa_family == AF_INET) {
        char addr[INET_ADDRSTRLEN];
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
            std::cerr
                << "Unalbe to retrieve IPv4 netmask address from the interface"
                << _name << std::endl;
          } else {
            address.netmask = addr;
            _ipv4_addresses.push_back(address);
          }
        }
      } else if (ifa->ifa_addr->sa_family == AF_INET6) {
        char addr[INET6_ADDRSTRLEN];
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

void NetworkInterface::SetMacAddress() {
  int fd;
  struct ifreq ifr;
  fd = socket(AF_INET, SOCK_DGRAM, 0);

  ifr.ifr_addr.sa_family = AF_INET;
  strncpy((char *)ifr.ifr_name, (const char *)_name.c_str(), IFNAMSIZ - 1);

  ioctl(fd, SIOCGIFHWADDR, &ifr);

  close(fd);

  uint8_t *mac = (uint8_t *)ifr.ifr_hwaddr.sa_data;

  char mac_hex[32];
  sprintf((char *)mac_hex, (const char *)"%.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n",
          mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  _mac = std::string(mac_hex);
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

std::string NetworkInterface::GetMacAddress() { return _mac; }