#ifndef PACKET_H
#define PACKET_H #include <pcap/pcap.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/ip_icmp.h>
#include <netinet/if_ether.h>
#include <arpa/inet.h>
#include <string>
#include <cstring>
#include <iostream>
#include <bitset>
#include <stdexcept>
// the wonderful power of being able to store unsigned chars in an object
// say goodbye to memcpy and strlen!!

typedef std::basic_string<u_char> ustring;
typedef std::bitset<4> short_bit;
typedef std::bitset<8> l_short_bit;

class packet { 
  friend std::ostream& operator<<(std::ostream& os, packet& p);
  public:
    packet() { } // needed for use in std::map
    packet(const u_char * p, struct timeval ts, unsigned int cap_len);
    bool syn() const;
    bool fin() const;
    bool rst() const;
    bool ack() const;
    tcp_seq ack_number() const;
    tcp_seq seq_number() const;
    bool complete() const { return completed; }
    u_short src_port() const;
    u_short dst_port() const;
    std::string src_addr() const;
    std::string dst_addr() const;
    u_short window_size();
    int size();
    int data_offset();
    time_t ts_sec() const;
    suseconds_t ts_milli() const;
    suseconds_t ts()  const; 
    ustring get_data() const;
    unsigned int data_size() const;

    /* ICMP Protocol stuff */
    u_int8_t get_icmp_type();
    std::string get_router(); // returns the ip address of the router that dropped the icmp packet; throws bad_packet_error otherwise 
  private:
    /* ICMP Protocol stuff */
    u_int8_t type;
    std::string router;

    ustring p_string; // string varient of the packet
    ustring data; // string varient of the data
    unsigned int capture_length; // length of packet
    unsigned int d_size; // length of packet
    std::string saddr; // ip addresses
    std::string daddr;
    l_short_bit flags;
    u_short sport; // port numbers
    u_short dport;
    u_short win; // window size
    tcp_seq ack_num;
    tcp_seq seq_num;
    u_char ip_hdr_len;
    u_char tcp_hdr_len;
    time_t time_stamp_sec;
    suseconds_t time_stamp_milli;
    bool completed = false; // flag is set of packet is not malformed
    bool has_data = true;
    bool too_short(unsigned int length); // packet length check
};

class bad_packet_error : public std::exception {
  public:
    bad_packet_error(const char * m) { method += std::string(m); }
    
    char const *what() const noexcept{
      return method.c_str();
    }
  private:
    std::string method = "Malformed_packet: " ;
};
std::ostream& operator<<(std::ostream& os, packet& p);
void ConvertToBinary(int n);
u_short short_swap( u_short s );
#endif
