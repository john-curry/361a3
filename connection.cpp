#include "connection.h"
#include "state.h"
#include <cassert>
#include <algorithm>
#include <cmath>

connection::connection(packet p) {
  this->src_addr         = p.src_addr(); 
  this->dst_addr         = p.dst_addr();
  this->src_port         = p.src_port(); 
  this->dst_port         = p.dst_port();
  this->connection_reset = p.rst();
  this->id               = p.ip_id;
  this->start_time = p.ts_milli() + p.ts_sec()*1000000;
  this->state = std::shared_ptr<connection_state>(new connection_start);
  assert(check_packet(p));
  this->recv_packet(p);
}

bool connection::check_packet(packet p) {
  assert(!(this->src_to_dst(p) && this->dst_to_src(p)));

  if (this->udp_packets[p.dst_port()]) return true;

  if (this->rtt_start.count(p.ip_id) > 0) return true;

  if (p.get_icmp_type() == 11 && p.dst_addr() == this->src_addr) return true;

  if ((p.src_addr() == this->src_addr) && (p.dst_addr() == this->dst_addr)) return true;

  return this->src_to_dst(p) ^ this->dst_to_src(p);
}

void connection::recv_packet(packet p) {
  using namespace std;
  assert(this->check_packet(p));
  if (p.ip_type == 17) this->num_udp_packets++;
  if (p.ip_type == 1 ) this->num_icmp_packets++;

  if (p.ip_type == 17) {
      this->udp_packets[p.dst_port()] = true;
      this->udp_packets[p.dst_port() + 1] = true;
      this->udp_packets[p.dst_port() + 2] = true;
  }

  if ((p.ttl == 1 && p.more_fragments()) /*&& (p.get_icmp_type() == ICMP_ECHO)*/) {
    this->fragments++;
  }

  if (p.ttl == 1 && !p.more_fragments() /*&& ((p.get_icmp_type() == ICMP_ECHO) || p.ip_type == 17)*/) {
    this->last_fragment_offset = p.fragment_number;
  }

  // rrt start timestamp when echo 
  if (p.ip_type == 17 || (p.ip_type == 1 && p.get_icmp_type() == ICMP_ECHO)) {
    rtt_start[p.ip_id] = p.ts();
  }
  
  if (p.ip_type == 1 && p.get_icmp_type() == ICMP_TIME_EXCEEDED) {
    auto it = rtt_start.find(p.ip_id);
    //assert(it != rtt_start.end());
    if (it != rtt_start.end()) {
      rtts.push_back(abs(p.ts() - it->second));
    }
  }

  this->state->recv_packet(p, this);
}

float connection::average_rtt() {
  float sum = 0;
  for (const auto rtt : rtts) {
    sum += rtt;
  }

  assert(sum >= 0);
  if (rtts.size() == 0) return 0;
  return sum / rtts.size();
}

float connection::standard_deviation_rtt() {
  using namespace std;
  float sum = 0;
  for (const auto rtt : rtts) {
    sum += (rtt - this->average_rtt())*(rtt - this->average_rtt());
  }
  assert(sum >= 0);
  if (rtts.size() == 0) return 0;
  float variance = sum / rtts.size();
  assert(variance >= 0);
  return sqrt(variance);
}


void connection::do_rtt_calculation(packet p) {
  assert(this->check_packet(p));
}

void connection::do_byte_calculation(packet p) {
  assert(this->check_packet(p));
}

void connection::do_packet_calculation(packet p) {
  assert(this->check_packet(p));
}

bool connection::is_completed() { return this->complete; }

void connection::set_completed(bool c) { this->complete = c; }

void connection::set_reset(bool r) { this->connection_reset = r; }

bool connection::is_reset() { return connection_reset; }

void connection::change_state(std::shared_ptr<connection_state> s) {
  this->state = s;
}

bool connection::src_to_dst(packet p) {
  bool addr = (p.src_addr() == this->src_addr) && (p.dst_addr() == this->dst_addr);
  bool port = (p.src_port() == this->src_port) && (p.dst_port() == this->dst_port);
  return addr && port;
}

bool connection::dst_to_src(packet p) {
  bool addr = (p.dst_addr() == this->src_addr) && (p.src_addr() == this->dst_addr);
  bool port = (p.dst_port() == this->src_port) && (p.src_port() == this->dst_port);
  return addr && port;
}

void connection::set_end_time(suseconds_t t) {
  this->end_time = t;
}

float connection::start() {
  return (float)(this->start_time - this->beginning)/((float)(1000000));
}

float connection::end() {
  return (float)(this->end_time - this->beginning)/((float)(1000000));
}

float connection::duration() {
  return (float)(this->end_time - this->start_time)/((float)(1000000));
}

void connection::configure_timestamp(suseconds_t begin) {
  this->beginning = begin;
}

bool connection::reseted() { return this->connection_reset; }

std::string connection::state_name() { return state->name(); }

void connection::add_to_route(uint8_t ttl, std::string address) {
  using namespace std;
  if (find(route.begin(), route.end(), address) == route.end()) {
    route.push_back(address);
  }
}

