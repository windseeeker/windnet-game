#ifndef __WINDNET_PEER_CONNECTION_H__
#define __WINDNET_PEER_CONNECTION_H__

#include "socket.h"

namespace Windnet {
namespace Peer {
class PeerConnection : public Socket {
public:


private:
	int m_serverId;
};
}
}

#endif
