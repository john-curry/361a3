#ifndef CONNECTIONS_H
#define CONNECIIONS_H
#include "connection.h"
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <functional>
class connections {
  friend std::ostream& operator<<(std::ostream& os, connections& p);
  private:
    std::vector<connection*> conns;
    suseconds_t beginning;
    size_t resets = 0;
  public:
    connections() { };
    void add_connection(connection * c) {
      conns.push_back(c);
    };

    bool is_new_connection(packet p) {
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

   std::vector<connection*> incomplete_connections() {
     std::vector<connection*> ret = conns;
     return ret;
   };
   std::vector<connection*> complete_connections() {
     std::vector<connection*> ret = conns;
     return ret;
   };

   size_t complete_size() { return this->complete_connections().size(); };
   size_t incomplete_size() { return this->incomplete_connections().size(); };

   std::vector<int> packets() {
     std::vector<int> ret;
     return ret;
   };

   std::vector<suseconds_t> rtts() {
     std::vector<suseconds_t> ret;
     return ret;
   };

   std::vector<u_short> windows() {
     using namespace std;
     auto comp = this->complete_connections();
     std::vector<u_short> ret;
     for (auto i : comp) {
       ret.insert(ret.end(), i->window_sizes.begin(), i->window_sizes.end());
     }
     return ret;
   };

   std::vector<float> times() {
     using namespace std;
     auto comp = this->complete_connections();
     std::vector<float> ret;
     for (auto i : comp) {
       ret.push_back(i->duration());
     }
     return ret; 
   };

   u_short min_window() {
     using namespace std;
     auto p = this->windows();
     return p[distance(p.begin(), min_element(p.begin(), p.end()))];
   }

   u_short max_window() {
     using namespace std;
     auto p = this->windows();
     return p[distance(p.begin(), max_element(p.begin(), p.end()))];
   }

   u_short mean_window() {
     auto t = this->windows();
      float ret = 0;
      for (auto i : t) {
        ret += i;
      }
      return ret/t.size();
   }

   double min_rtt() {
     using namespace std;
     auto p = this->rtts();
     return (double)p[distance(p.begin(), min_element(p.begin(), p.end()))]/1000000.0;
   }

   double max_rtt() {
     using namespace std;
     auto p = this->rtts();
     return (double)p[distance(p.begin(), max_element(p.begin(), p.end()))]/1000000.0;
   }

   float mean_rtt() {
     auto t = this->rtts();
      float ret = 0;
      for (auto i : t) {
        float secs = (float)i / 1000000.0;
        ret += secs;
      }
      return (float)ret/(float)t.size();
   }

   int min_packet() {
     using namespace std;
     auto p = this->packets();
     return p[distance(p.begin(), min_element(p.begin(), p.end()))];
   }

   int max_packet() {
     using namespace std;
     auto p = this->packets();
     return p[distance(p.begin(), max_element(p.begin(), p.end()))];
   }

   int mean_packet() {
     auto t = this->packets();
      float ret = 0;
      for (auto i : t) {
        ret += i;
      }
      return ret/t.size();
   }

   float min_time() {
     auto t = this->times();
     return t[std::distance(t.begin(), std::min_element(t.begin(), t.end()))];
     
   }

   float max_time() {
     auto t = this->times();
     return t[std::distance(t.begin(), std::max_element(t.begin(), t.end()))];
   }

   float mean_time() {
     auto t = this->times();
      float ret = 0;
      for (auto i : t) {
        ret += i;
      }
      return ret/t.size();
   }
};

#endif
