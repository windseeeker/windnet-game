#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <iostream>

#include "log.h"
#include "ring_queue.h"
#include "player_session.h"
#include "socket.h"
#include "socket_server.h"

#include "epoll_server.h"


namespace Windnet {
namespace Net {
int SocketServer::initServer() {
	m_epServer = new EpollServer(m_resource);
	m_epServer->init();
	return 0;
}

int SocketServer::startServer() {
	return m_epServer->epollRoutine();
}

void SocketServer::addEvent(int fd, void *ptr) {
	m_epServer->add(fd,  ptr);
}

void *SocketServer::socketAcceptRoutine(void *arg) {
	SockThreadParam *threadParam = (SockThreadParam*)arg;
	SocketServer *sockServer = threadParam->sockServer;
	assert(sockServer);

	Socket *listen = Socket::createSocket();

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(threadParam->port);
	addr.sin_addr.s_addr = INADDR_ANY;

	int val = 1;
	listen->setSockOpt(SOL_SOCKET, SO_REUSEADDR, val);
	listen->bind(&addr);
	listen->listen();

	sockaddr_in sockAddr;
	socklen_t sockLen = sizeof(sockAddr);
	for (; ;) {
		int fd = ::accept(listen->fd(), (sockaddr*)&sockAddr, &sockLen);
		if (fd == -1) {
			return NULL;
		}
		PlayerSession *ps = new PlayerSession;
		ps->fd(fd);

		printf("ps connection id %d\n", ps->fd());
		//Socket *sock = listen->accept();
		ps->serviceType(threadParam->serviceType);
		fprintf(stdout, "New connection\n");
		fflush(stdout);
		sockServer->addEvent(ps->fd(), ps);
	}
}
}
}
