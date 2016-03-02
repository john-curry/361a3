#include "packet.h"
#include <cassert>
#include <endian.h>
#include <stdint.h>
packet::packet(const u_char * p, struct timeval ts, unsigned int cap_len) {
  using namespace std;

  struct ip *mip;
  struct icmphdr *micmp;

  unsigned int eth_header_length = sizeof(struct ether_header); 
  unsigned int ip_header_length;
  unsigned int icmp_header_length;

  this->p_string = ustring(p);
  this->time_stamp_sec = (time_t)ts.tv_sec;
  this->time_stamp_milli = ts.tv_usec;
  this->capture_length = cap_len;

  if (too_short(eth_header_length)) {
    cerr << "Captured packet too small: ethhdr." << endl;
  } else { 
    // jump over ethernet header
    p += eth_header_length;
    cap_len -= eth_header_length;
    
    mip = (struct ip*)p;
    // rip addresses out of ip header    
    this->saddr = string(inet_ntoa(mip->ip_src));
    this->daddr = string(inet_ntoa(mip->ip_dst));
    assert(this->saddr != this->daddr);
    ip_header_length = mip->ip_hl * 4;

    assert(ip_header_length >= 20 && ip_header_length <= 60);

    this->ip_hdr_len = ip_header_length;
     
    if (too_short(ip_header_length)) {
      cerr << "Captured packet too small: iphdr." << endl;
    } else {
      // jump over ip header to the tcp header
      p += ip_header_length;
      cap_len -= ip_header_length;

      micmp = (struct icmphdr*)p;

      icmp_header_length = sizeof(struct icmphdr);
      this->type = micmp->type;
      if (micmp->type != ICMP_TIME_EXCEEDED) {
        /* do ping analysis of some sort */
      } else {
        /* do rip gateway address out of packet */
        p += 8;
        cap_len -= 8;

        struct ip * ip_header = (struct ip*)p;
        this->router = this->saddr;
        this->daddr = string(inet_ntoa(ip_header->ip_src));
        this->saddr = string(inet_ntoa(ip_header->ip_dst));
        /* CONVENTION: if this is a type 11 packet, treat it as an incoming packet from the original destination  */ 
        //this->daddr = string(inet_ntoa(icmp_packet->icmp_ip.ip_dst));
        //this->saddr = string(inet_ntoa(icmp_packet->icmp_ip.ip_src));
        
      }
     
    }
  }
  this->completed = true;
}

ustring packet::get_data() const {
  if (!completed) throw bad_packet_error("get_data");
  return data;
}

bool packet::too_short(unsigned int length) {
  if (capture_length < length) return true;
  return false;
}
    
std::string packet::src_addr() const {
  if (!completed) throw bad_packet_error("src_addr");
  return saddr;
}

std::string packet::dst_addr() const {
  if (!completed) throw bad_packet_error("dst_addr");
  return daddr;
}

u_short packet::dst_port() const {
  if (!completed) throw bad_packet_error("dst_port");
 return dport;
}

u_short packet::src_port() const {
  if (!completed) throw bad_packet_error("src_port");
 return sport;
}

bool packet::fin() const {
  return (bool)flags[0];
}

bool packet::syn() const {
  return (bool)flags[1];
}

bool packet::rst() const {
  return (bool)flags[2];
}

bool packet::ack() const {
  return (bool)flags[4];
}

tcp_seq packet::ack_number() const {
  return ack_num;
}

tcp_seq packet::seq_number() const {
  return seq_num;
}

time_t packet::ts_sec() const { 
  return time_stamp_sec;
}

suseconds_t packet::ts_milli() const { 
  return time_stamp_milli;
}

suseconds_t packet::ts() const {
  return this->ts_milli() + this->ts_sec()*1000000;
}

unsigned int packet::data_size() const { 
  return this->d_size;
}

u_short packet::window_size() {
  return this->win;
}

u_int8_t packet::get_icmp_type() {
  return this->type;
}

std::string packet::get_router() {
  if (this->type != ICMP_TIME_EXCEEDED) throw bad_packet_error("This packet is not a traceroute packet.");
  return this->router;
}

std::ostream& operator<<(std::ostream& os, packet& p) {
  os << " src_addr: " << p.src_addr()
     << " dst_addr: " << p.dst_addr()
     << " icmp_type: " << (int)p.get_icmp_type()
     ;
     if (p.get_icmp_type() == 11) {
       os << " gateway_addr: " << p.get_router()
       ;
     }
     //<< " ack_num: " << (p.ack_num)
     //<< " seq_num: " << (p.seq_num)
     //<< " ack: " << p.ack()
     //<< " syn: " << p.syn()
     //<< " has data: " << p.has_data;
     //if (p.has_data) {
     //  os << " data size: " << p.data_size();
     //}
     ;
  return os;
}

u_short short_swap( u_short s ) {
  unsigned char b1, b2;
  
  b1 = s & 255;
  b2 = (s >> 8) & 255;

  return (b1 << 8) + b2;
}
void ConvertToBinary(int n) {
  if (n / 2 != 0) {
  ConvertToBinary(n / 2);
  }
  printf("%d", n % 2);
}
