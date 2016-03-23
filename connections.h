#ifndef CONNECTIONS_H
#define CONNECIIONS_H
#include "connection.h"
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <functional>
class connections {
  friend std::ostream& operator<<(std::ostream& os, connections*& p);
  private:
    std::vector<connection*> conns;
    suseconds_t beginning;
  public:
    connections() { };
    void add_connection(connection * c) {
      conns.push_back(c);
    };

    bool is_new_connection(packet p) {
      if (p.fragment_number != 0) return false;

      if ((p.ttl == 1) && (p.ip_type == 17)) {
        return true;
      }

      if ((p.ttl == 1) && (p.get_icmp_type() == 8)) {
        return true;
      }

      return false;
    };

    void recv_packet(packet p) {
      assert(!this->is_new_connection(p));
      for (auto c: conns) {
        if (c->check_packet(p)) {
          c->recv_packet(p);
        }
      }
    };

    bool empty() { return conns.size() == 0 ? true : false; };
    void start_time(suseconds_t b) { this->beginning = b; };

};

#endif
