#ifndef __SOCKET_SERVER_H__
#define __SOCKET_SERVER_H__

#include <boost/shared_ptr.hpp>

class ServerResource;

namespace Windnet {
namespace Net {
class Socket;
class EpollServer;

class SocketServer {
public:
	struct SockThreadParam {
		SocketServer *sockServer;
		char serviceType;
		int port;
	};
    SocketServer(boost::shared_ptr<ServerResource> res):m_resource(res) { }
	boost::shared_ptr<ServerResource>& serverResource() { return m_resource; }

	int startServer();
	int initServer();

	void addEvent(int fd, void *ptr);
	static void *socketAcceptRoutine(void *arg);

private:
	EpollServer *m_epServer;
	boost::shared_ptr<ServerResource> m_resource;
};
}
}

#endif
