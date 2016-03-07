#ifndef STATE_H
#define STATE_H
#include "packet.h"
class connection;

class connection_state {
  public:
    virtual std::string name() const = 0;
    virtual void recv_packet(packet p, connection * c) = 0;
};

class packet_sent : public connection_state  { std::string name() const override; void recv_packet(packet,connection*); };
class trace_start : public connection_state  { std::string name() const override; void recv_packet(packet,connection*); };
class packet_recvd : public connection_state { std::string name() const override; void recv_packet(packet,connection*); };




// traceroute start
// traceroute ttl = 1
// return icmp
// traceroute ttl = 2
// return icmp 
//  ...
// traceroute ttl = i
// return icmp
//  ...
// traceroute ttl = n
// return from ulimate destination
// traceroute done

std::ostream& operator<<(std::ostream& os, const connection_state& c);
#endif
