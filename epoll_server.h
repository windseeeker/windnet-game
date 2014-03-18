#ifndef __EPOLL_SERVER_H__
#define __EPOLL_SERVER_H__

#include <boost/shared_ptr.hpp>

class ServerResource;

namespace Windnet {
namespace Net {
class EpollServer {
public:
    EpollServer(boost::shared_ptr<ServerResource> &res):m_serverRes(res) { }

	int init();
	int add(int fd, void *ptr);
	int remove(void *ptr);

	int epollRoutine();

private:
	int m_epfd;
	boost::shared_ptr<ServerResource> &m_serverRes;
};
}
}

#endif
