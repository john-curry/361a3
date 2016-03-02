#ifndef CONNECTION_H
#define CONNECTION_H
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <cassert>
#include <map>
#include "packet.h"
#include "state.h"

class connection {
  friend std::ostream& operator<<(std::ostream& os, connection& p);
  friend class connections;
  public:

    connection(packet p);

    void change_state(std::shared_ptr<connection_state> s);
    bool check_packet(packet p); void recv_packet(packet p);
    bool is_completed();
    void set_completed(bool);
    bool reseted();
    void set_reset(bool r);
    bool is_reset();
    bool src_to_dst(packet p);
    bool dst_to_src(packet p);
    void set_end_time(time_t);
    float start();
    float end();
    float duration();
    void configure_timestamp(suseconds_t);
    void do_rtt_calculation(packet);
    void do_byte_calculation(packet);
    void do_packet_calculation(packet);
    std::string state_name();
  private:
    // start state with no syn and no fin
    std::string src_addr;
    std::string dst_addr;
    int src_port;
    int dst_port;
    suseconds_t start_time;
    suseconds_t end_time;
    int packet_src_to_dst_num = 0;
    int packet_dst_to_src_num = 0;
    int packet_num = 0;

    int byte_src_to_dst_num = 0;
    int byte_dst_to_src_num = 0;
    int byte_total = 0;

    tcp_seq seq_num = 0;
    tcp_seq nxt_ack = 0;
    tcp_seq ack_num = 0;
    suseconds_t rtt_t0;
    std::vector<u_short> window_sizes;
    std::vector<suseconds_t> rtts;
    std::map<tcp_seq, packet> src_packets; // a packet is located at the ack number it is waiting for
    std::map<tcp_seq, packet> dst_packets; // a packet is located at the ack number it is waiting for
    suseconds_t beginning;
    std::shared_ptr<connection_state> state;
    bool complete = false;
    bool connection_reset = false;
    bool fin_set = false;
};
#endif
