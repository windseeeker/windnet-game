#ifndef __GAME_SERVER_H__
#define __GAME_SERVER_H__

//#include "boost/shared_ptr.hpp"

class PacketDispatch;
class ServerResource;

namespace Windnet {
namespace Net {
	class SocketServer;
}

namespace Mysql {
	class DBConnectin;
	class DBConnectionPool;
}

}

class GameServer {
public:
	GameServer();
	~GameServer();

	Windnet::Net::SocketServer *sockServer() const { return m_sockServer; }

	ServerResource *serverRes() { return m_resource; }

	void init();
	void start();

private:
	Windnet::Net::SocketServer *m_sockServer;
	ServerResource *m_resource;
};

#endif
