#ifndef M_PRINT_H
#define M_PRINT_H
#include "packet.h"
//#include "connections.h"
#include <iostream>
class connection;
class connections;
std::ostream& operator<<(std::ostream& os, packet& p);
std::ostream& operator<<(std::ostream& os, connection*& c);
std::ostream& operator<<(std::ostream& os, connections*& p);
#endif
