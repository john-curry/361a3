#include <iostream>
#include "print.h"
#include "connection.h"
#include "connections.h"
std::ostream& operator<<(std::ostream& os, packet& p) {
  os << " src_addr: " << p.src_addr()
     << " dst_addr: " << p.dst_addr()
     << " ip_type: " << (int)p.ip_type
     << " ip_id: "   << (int)p.ip_id
     << " icmp_type: " << (int)p.get_icmp_type()
     << " ip_ttl: " << (int)p.ttl
     ;
     if (p.get_icmp_type() == 11) {
       os << " gateway_addr: " << p.get_router()
          << " return_ttl: " <<(int)p.get_return_ttl()
       ;
     }
     ;
  return os;
}

std::ostream& operator<<(std::ostream& os, connection*& c) {
  using namespace std;
  
  os << "Source node: "      << c->src_addr << endl
     << "Destination node: " << c->dst_addr << endl
     << "IP addresses of the intermediate destination nodes: " << endl
     ;
  int i = 1;
  for (auto r : c->route) {
    os << "router " << i++ << ": " << r  << "," << endl;
  }
  os << endl;

  os << "The values in the protocol field of IP headers: " << endl
     << c->num_icmp_packets << ": ICMP" << endl
     << c->num_udp_packets  << ": UDP " << endl << endl
     << "The number of fragments created from the original datagram is: " << c->fragments << endl
     << "The offset of the last fragment is: " << (c->last_fragment_offset * 8) << endl << endl
     ;
  return os;
}

std::ostream& operator<<(std::ostream& os, connections*& p) {
  using namespace std;
  int i = 1;
  for (auto c : p->conns) {
    os << "Connection " << i++ << " " << endl
       << c << endl;
  }
  for (const auto & c : p->conns) {
    os << "The avg RTT between " << c->src_addr 
       << " and "                << c->dst_addr 
       << " is: "                << (c->average_rtt()  / 1000000.0)
       << " ms, the s.d. is: "   << (c->standard_deviation_rtt() / 1000000.0)
       << " ms"                  << endl
       ;
  }
  return os;
}
