#ifndef __GAME_SERVER_H__
#define __GAME_SERVER_H__

#include "boost/shared_ptr.hpp"

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

	boost::shared_ptr<Windnet::Net::SocketServer> sockServer() const { return m_sockServer; }

	boost::shared_ptr<ServerResource> serverRes() { return m_resource; }

	void init();
	void start();

private:
	boost::shared_ptr<Windnet::Net::SocketServer> m_sockServer;
	boost::shared_ptr<ServerResource> m_resource;
};

#endif
