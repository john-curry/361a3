#include "state.h"
#include "connection.h"
std::string reset::name() const { return std::string("reset"); }
void reset::recv_packet( packet p, connection * c) {
  c->set_reset(true);
  c->set_end_time(p.ts_milli() + p.ts_sec()*1000000);
}

std::string s0f0::name() const { return std::string("s0f0"); }
void s0f0::recv_packet( packet p, connection * c) {
  using namespace std;
  assert(!c->is_completed());
  if(p.fin() && p.syn()) {
    c->change_state(shared_ptr<s1f1>(new s1f1));
  }
  if (p.syn() && !p.fin()) {
    c->change_state(shared_ptr<s1f0>(new s1f0));
  }
  if(p.fin() && !p.syn()) {
    c->change_state(shared_ptr<s0f1>(new s0f1));
  }
}

std::string s1f1::name() const { return std::string("s1f1"); }
void s1f1::recv_packet( packet p, connection * c) {
  using namespace std;
  assert(!c->is_completed());
  c->set_completed(true);
  c->set_end_time(p.ts_milli() + p.ts_sec()*1000000);
  if (p.syn() && p.fin()) {
    c->change_state(shared_ptr<s2f2>(new s2f2));
  }
  if (p.syn() && !p.fin()) {
    c->change_state(shared_ptr<s2f1>(new s2f1));
  }
  if (p.fin() && !p.syn()) {
    c->change_state(shared_ptr<s1f2>(new s1f2));
  }
}

std::string s1f0::name() const { return std::string("s1f0"); }
void s1f0::recv_packet( packet p, connection * c) {
  using namespace std;
  assert(!c->is_completed());
  if (p.syn() && p.fin()) {
    c->change_state(shared_ptr<s2f1>(new s2f1));
  }
  if (p.syn() && !p.fin()) {
    c->change_state(shared_ptr<s2f0>(new s2f0));
  }
  if (p.fin() && !p.syn()) {
    c->change_state(shared_ptr<s1f1>(new s1f1));
  }
}

std::string s2f1::name() const { return std::string("s2f1"); }
void s2f1::recv_packet( packet p, connection * c) {
  using namespace std;
  c->set_end_time(p.ts_milli() + p.ts_sec()*1000000);
  c->set_completed(true);
  if (p.fin()) {
    c->change_state(shared_ptr<s2f2>(new s2f2));
  }
}

std::string s0f1::name() const { return std::string("s0f1"); }
void s0f1::recv_packet( packet p, connection * c) {
  using namespace std;

  assert(!c->is_completed());
  if (p.syn() && p.fin()) {
    c->change_state(shared_ptr<s1f2>(new s1f2));
  }
  if (p.syn() && !p.fin()) {
    c->change_state(shared_ptr<s1f1>(new s1f1));
  }
  if (p.fin() && !p.syn()) {
    c->change_state(shared_ptr<s0f2>(new s0f2));
  }
}

std::string s1f2::name() const { return std::string("s1f2"); }
void s1f2::recv_packet( packet p, connection * c) {
  using namespace std;
  c->set_completed(true);
  c->set_end_time(p.ts_milli() + p.ts_sec()*1000000);
  if (p.syn()) {
    c->change_state(shared_ptr<s2f2>(new s2f2));
  }
}

std::string s0f2::name() const { return std::string("s0f2"); }
void s0f2::recv_packet( packet p, connection * c) {
  using namespace std;
  assert(!c->is_completed());
  if (p.syn()) {
    c->change_state(shared_ptr<s1f2>(new s1f2));
  }
}

std::string s2f0::name() const { return std::string("s2f0"); }
void s2f0::recv_packet( packet p, connection * c) {
  using namespace std;
  assert(!c->is_completed());
  if (p.fin()) {
    c->change_state(shared_ptr<s2f1>(new s2f1));
  }
}

std::string s2f2::name() const { return std::string("s2f2"); }
void s2f2::recv_packet( packet p, connection * c) {
  using namespace std;
  c->set_end_time(p.ts_milli() + p.ts_sec()*1000000);
  assert(c->is_completed()); 
}

std::ostream& operator<<(std::ostream& os, const connection_state& c) {
  os << c.name();
  return os;
}
