#include "connection.h"
#include "state.h"
#include <cassert>
#include <algorithm>

connection::connection(packet p) {
  this->src_addr         = p.src_addr(); 
  this->dst_addr         = p.dst_addr();
  this->src_port         = p.src_port(); 
  this->dst_port         = p.dst_port();
  this->connection_reset = p.rst();
  this->start_time = p.ts_milli() + p.ts_sec()*1000000;
  this->state = std::shared_ptr<connection_state>(new connection_start);
  assert(check_packet(p));
  this->recv_packet(p);
}

bool connection::check_packet(packet p) {
  assert(!(this->src_to_dst(p) && this->dst_to_src(p)));

  if (p.get_icmp_type() == 11 && p.dst_addr() == this->src_addr) return true;

  return this->src_to_dst(p) ^ this->dst_to_src(p);
}

void connection::recv_packet(packet p) {
  using namespace std;
//  assert(this->check_packet(p));
//
//  this->window_sizes.push_back(p.window_size());
//
//  this->do_packet_calculation(p);
//
//  this->do_byte_calculation(p);
//
//  this->do_rtt_calculation(p);
//
//  if (p.rst()) {
//    this->connection_reset = true;
//  } 
//
  this->state->recv_packet(p, this);
}

void connection::do_rtt_calculation(packet p) {
  assert(this->check_packet(p));

  if (this->src_to_dst(p)) {
    if (p.syn() && !p.ack()) {
      this->seq_num = p.seq_number();
      auto it = dst_packets.find(p.seq_number());
      if (it != dst_packets.end()) {
        dst_packets.erase(it);
        this->rtts.push_back(p.ts() - it->second.ts());
      }
      this->nxt_ack = p.seq_number() + 1;
      src_packets[this->nxt_ack] = p;
    }
    if (p.ack() && !p.syn()) {
      this->seq_num = p.seq_number();
      auto it = dst_packets.find(p.seq_number());
      if (it != dst_packets.end()) {
        dst_packets.erase(it);
        this->rtts.push_back(p.ts() - it->second.ts());
      }
      this->nxt_ack = p.seq_number() + p.data_size();
      src_packets[this->nxt_ack] = p;
    }
    if (p.fin()) {
      this->fin_set = true;
      this->rtt_t0 = p.ts();
    }
  }
  
  if (this->dst_to_src(p)) {
    if (p.ack() && p.fin()) {
      if (this->fin_set) {
        this->rtts.push_back(p.ts() - this->rtt_t0);
      }
    }
    if (p.syn() && p.ack()) {
      auto it = src_packets.find(p.ack_number());
      if (it != src_packets.end()) {
        src_packets.erase(it);
        this->rtts.push_back(p.ts() - it->second.ts());
      }
      dst_packets[p.ack_number()] = p;
    }
    if (p.ack() && !p.syn()) {
      auto it = src_packets.find(p.ack_number());
      if (it != src_packets.end()) {
        src_packets.erase(it);
        this->rtts.push_back(p.ts() - it->second.ts());
      }
      dst_packets[p.ack_number()] = p;
    }
  }
}

void connection::do_byte_calculation(packet p) {
  assert(this->check_packet(p));

  this->byte_total += p.data_size();

  if (this->src_to_dst(p)) {
    this->byte_src_to_dst_num += p.data_size();
    return;
  }
  if (this->dst_to_src(p)) {
    this->byte_dst_to_src_num += p.data_size();
    return;
  }
  assert(false);
}

void connection::do_packet_calculation(packet p) {
  assert(this->check_packet(p));
  this->packet_num++;

  if (this->src_to_dst(p)) {
    this->packet_src_to_dst_num++;
    return;
  }

  if (this->dst_to_src(p)) {
    this->packet_dst_to_src_num++;
    return;
  }
  assert(false);
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

void connection::add_to_route(std::string address) {
  using namespace std;
  if (find(route.begin(), route.end(), address) == route.end()) {
    route.push_back(address);
  }
}

