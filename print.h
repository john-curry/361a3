#ifndef M_PRINT_H
#define M_PRINT_H
#include "packet.h"
#include <iostream>
std::ostream& operator<<(std::ostream& os, packet& p) {
  os << " src_addr: " << p.src_addr()
     << " dst_addr: " << p.dst_addr()
     << " ip_type: " << (int)p.ip_type
     //<< " more_coming?: " << p.more_frags
     << " icmp_type: " << (int)p.get_icmp_type()
     << " ip_ttl: " << (int)p.ttl
     ;
     if (p.get_icmp_type() == 11) {
       os << " gateway_addr: " << p.get_router()
       ;
     }
     ;
  return os;
}

std::ostream& operator<<(std::ostream& os, connection& c) {
  using namespace std;
  os << "Source node: " << c.src_addr << endl
     << "Destination node: " << c.dst_addr << endl
     << "IP addresses of the intermediate destination nodes: " 
     << endl;
  int i = 1;
  for (auto r : c.route) {
    os << "router " << i++ << ": " << r  << "," << endl;
  }
  return os;
}

std::ostream& operator<<(std::ostream& os, connections& p) {
  using namespace std;
  for (auto c : p.conns) {
    os << *c << endl;
  }
  return os;
}
#endif
