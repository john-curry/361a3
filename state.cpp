#include "state.h"
#include "connection.h"

template<class T>
void change_connection_state(connection * c) {
  c->change_state(std::shared_ptr<connection_state>(new T));
}

std::string connection_start::name() const { return "paclet_name"; }
void connection_start::recv_packet(packet p, connection * c) {
  assert((p.get_icmp_type() == 8 && p.ttl == 1) || (p.ip_type == 17 && p.ttl == 1)); // only start on icmp ping request
  change_connection_state<packet_sent>(c); // now wait for reply
}

std::string packet_recvd::name() const { return "packet_recvd"; }
void packet_recvd::recv_packet(packet p, connection * c) {
  //std::cout << "Revieved packet " << std::endl;
  //if (p.get_icmp_type() == 8) {
  change_connection_state<packet_sent>(c);
  //}
}


std::string packet_sent::name() const { return "packet_sent"; }
void packet_sent::recv_packet(packet p, connection * c) {
  if (p.get_icmp_type() == 11) {
    //std::cout << "Got address " << p.get_router() << std::endl;
    c->add_to_route(p.get_return_ttl(), p.get_router()); 
    //change_connection_state<packet_recvd>(c);
  }
}


std::string trace_start::name() const { return "trace_start"; }
void trace_start::recv_packet(packet p, connection * c) { 
  if (p.get_icmp_type() == 0) {
    //change_connection_state<packet_sent>(c);
  }
}
