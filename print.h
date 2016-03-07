#ifndef M_PRINT_H
#define M_PRINT_H
#include "packet.h"
#include <iostream>
std::ostream& operator<<(std::ostream& os, packet& p) {
  os << " src_addr: " << p.src_addr()
     << " dst_addr: " << p.dst_addr()
     << " more_coming?: " << p.more_frags
     << " frag_num: " << (p.fragment_number)
     << " ip_type: "  << (int)p.ip_type
     << " id: "       << p.identification
     << " ip_total_length: " << p.ip_total_length
     << " ip_hdr_length: " << (int)p.ip_hdr_len
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
#endif
