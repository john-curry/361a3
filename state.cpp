#include "state.h"
#include "connection.h"

template<class T>
void change_connection_state(connection * c) {
  c->change_state(new std::shared_ptr<T>(new T));
}

std::string packet_recvd::name() const { return "packet_recvd"; }

void packet_recvd::recv_packet(packet p, connection * c) {
  if (p.get_icmp_type() == 0) {
    //c->on_hop(route);
    //change_connection_state<packet_sent>(c);
  }
}


std::string packet_sent::name() const { return "packet_sent"; }
void packet_sent::recv_packet(packet p, connection * c) {
  if (p.get_icmp_type() == 11) {
    //c->on_hop(route);
    //change_connection_state<packet_recvd>(c);
  }
}


std::string trace_start::name() const { return "trace_start"; }
void trace_start::recv_packet(packet p, connection * c) { 
  if (p.get_icmp_type() == 0) {
    //change_connection_state<packet_sent>(c);
  }
}
