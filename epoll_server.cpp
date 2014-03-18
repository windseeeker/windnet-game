#include "epoll_server.h"

#include <sys/epoll.h>

#include <iostream>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "socket.h"

#include "message.h"
#include "ring_queue.h"
#include "player_session.h"
#include "server_resource.h"
#include "servlet.h"

#define DEFAULT_SOCKET_COUNT  2048 << 1

namespace Windnet {
namespace Net {
int EpollServer::init() {
	m_epfd = epoll_create(DEFAULT_SOCKET_COUNT);
	if (m_epfd == -1) {
		perror("epoll create error");
		exit(0);
    }
	return 0;
}

int EpollServer::add(int fd, void *ptr) {
	struct epoll_event ev;
	ev.data.ptr = ptr;
	ev.events = EPOLLIN | EPOLLET;
	return epoll_ctl(m_epfd, EPOLL_CTL_ADD, fd, &ev);
}

int EpollServer::remove(void *ptr) {
	struct epoll_event *ev = (epoll_event*)ptr;
	return epoll_ctl(m_epfd, EPOLL_CTL_DEL, ev->data.fd, ev);
}

int EpollServer::epollRoutine() {
	static epoll_event evs[DEFAULT_SOCKET_COUNT];

	while (1) {
		int n = epoll_wait(m_epfd, evs, DEFAULT_SOCKET_COUNT, -1);
		if (n == -1 && errno != EINTR) {
			perror("epoll wait");
			break;
		}
		for (int i = 0; i < n; ++i) {
			if (evs[i].events & EPOLLIN) {
				Socket *sock = (Socket*)evs[i].data.ptr;
				if (!sock) {
					epoll_ctl(m_epfd, EPOLL_CTL_DEL, sock->fd(), &evs[i]);
					//delete sock;
					fprintf(stdout, "Null epoll sock ptr\n");
					fflush(stdout);
					continue;
				}
				BsonBuffer &buffer = sock->readBuffer();
				if (sock->read(buffer) == -1) {
					sock->close();
					epoll_ctl(m_epfd, EPOLL_CTL_DEL, sock->fd(), &evs[i]);
					//delete sock;
					fprintf(stdout, "Read data error\n");
					fflush(stdout);
				}
				if (!buffer.parseable()) {
					continue;
				}
				if (buffer.parseFromBuffer(1) == -1) {
					sock->close();
					fprintf(stdout, "parse bson buffer error\n");
					continue;
				}
				PlayerSession *ps = dynamic_cast<PlayerSession*>(sock);
				assert(ps);

				Message *msg = new Message;
				msg->ps = ps;
				msg->request = buffer.protocol(); ////////////TODO .........
				m_serverRes->getQueue()->push(msg);
				//m_serverRes->getServletsDispatch()->dispatch(m_serverRes, ps, buffer.protocol());
			}
			if (evs[i].events & EPOLLOUT) {
				fprintf(stdout, "Null epoll sock ptr\n");
				fflush(stdout);
			}
		}
	}
	return 0;
}
}
}
