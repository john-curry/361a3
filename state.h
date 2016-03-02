#ifndef STATE_H
#define STATE_H
#include "packet.h"
class connection;
class connection_state {
  public:
    virtual std::string name() const = 0;
    virtual void recv_packet(packet p, connection * c) = 0;
};
class s0f0: public connection_state {
  public: 
    std::string name() const;
    void recv_packet( packet p, connection * c);
};
class s0f1: public connection_state {
  public:
    std::string name() const;
    void recv_packet( packet p, connection * c);
};
class s0f2: public connection_state {
  public:
    std::string name() const;
    void recv_packet( packet p, connection * c);
};
class s1f0: public connection_state {
  public:
    std::string name() const;
    void recv_packet( packet p, connection * c);
};
class s1f1: public connection_state {
  public:
    std::string name() const;
    void recv_packet( packet p, connection * c);
};
class s1f2: public connection_state {
  public:
    std::string name() const;
    void recv_packet( packet p, connection * c);
};
class s2f0: public connection_state {
  public:
    std::string name() const;
    void recv_packet( packet p, connection * c);
};
class s2f1: public connection_state {
  public:
    std::string name() const;
    void recv_packet( packet p, connection * c);
};
class s2f2: public connection_state {
  public:
    std::string name() const;
    void recv_packet( packet p, connection * c);
};
class reset: public connection_state {
  public:
    std::string name() const;
    void recv_packet( packet p, connection * c);
};

std::ostream& operator<<(std::ostream& os, const connection_state& c);
#endif
