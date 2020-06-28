#include <iostream>

#include <arpa/inet.h>
#include <ifaddrs.h>
#include <cstring>

#include "network.h"

NetworkInterface::NetworkInterface(std::string name) {
  _name = name;
  std::cout << "Find network interface: " << _name << std::endl;
  struct ifaddrs *interfaces = NULL;
  if (getifaddrs(&interfaces) != 0) {
    std::cerr << "Unalbe to retrieve network interfaces from the system!"
              << std::endl;
  }
  while (interfaces != NULL) {
    std::string str(interfaces->ifa_name);
    if (str == _name) {
      std::cout << "Found!" << std::endl;
      if (interfaces->ifa_addr->sa_family == AF_INET) {
        _ipv4_addresses.push_back(
            inet_ntoa(((struct sockaddr_in *)interfaces->ifa_addr)->sin_addr));
      } else {
        // TODO(thsioutas): Add to _ipv6_addresses
      }
    }
    interfaces = interfaces->ifa_next;
  }
}