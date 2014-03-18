#ifndef __SESSION_H__
#define __SESSION_H__

#include "socket.h"

namespace Windnet {
class Buffer;

class Session {
public:
	//void sendData(Windnet::Buffer &buffer) {}

private:
	Windnet::Net::Socket m_socket;
};

}
#endif
