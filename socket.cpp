#include "socket.h"

#include <iostream>

#include <unistd.h>
#include <stdio.h>
#include <errno.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "bson_buffer.h"
#include "message.h"
#include "ring_queue.h"

#include "servlet.h"
#include "server_resource.h"

namespace Windnet {
namespace Net {

void Socket::nonblocking() {
	int mask = fcntl(m_fd, F_GETFD, 0);
	mask |= O_NONBLOCK;
	fcntl(m_fd, F_SETFD, &mask);
}

Socket *Socket::createSocket() {
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0) {
		perror("socket");
		return NULL;
	}
	Socket *sock = new Socket(fd);
	return sock;
}

void Socket::setSockOpt(int level, int opt, int val) {
	setsockopt(m_fd, level, opt, &val, sizeof(val));
}

void Socket::close() {
	::close(m_fd);
}

void Socket::bind(sockaddr_in *addr) {
	socklen_t len = sizeof(*addr);
	if (::bind(m_fd, (sockaddr*)addr, len) == -1) {
		perror("bind error");
	}
}

void Socket::listen() {
	if (::listen(m_fd, 200) == -1) {
		perror("listen error");
	}
}

Socket *Socket::accept() {
	sockaddr_in sockAddr;
	socklen_t sockLen = sizeof(sockAddr);

	int fd = ::accept(m_fd, (sockaddr*)&sockAddr, &sockLen);
	if (fd == -1) {
		return NULL;
	}
	Socket *sock = new Socket(fd);
	sock->nonblocking();
	return sock;
}

bool Socket::sendData(Buffer &buffer) {
	write(buffer.buffer(), buffer.readAvailable());

	/*
	for (int i = 0; i < buffer.readAvailable(); ++i) {
		fprintf(stdout, "%02x ", buffer.buffer()[i]);
	}
	fprintf(stdout, "\n");
	*/
	return true;
}

int Socket::write(const char *buffer, size_t bufflen) {
	return ::write(m_fd, buffer, bufflen);
}

int Socket::read(BsonBuffer &buf, int toRead) {
	char buffer[2048];
	if (toRead == -1) {
		toRead = sizeof(buffer);
	}
	int length;
	do {
		length = ::read(m_fd, buffer, toRead);
		if (length == -1) {
			if (errno == EINTR) {
				fprintf(stdout, "read operation interupted\n");
				continue;
			} else if (errno == EAGAIN) {
				fprintf(stdout, "no data to read, errno is EAGAIN\n");
				break;
			}
			return -1;
		} else if (length == 0) {
			std::cerr << "peer closed connection" << std::endl;
			return -1;
		}
		buf.add(buffer, length);
		//m_readBuffer.add(buffer, length);
		fprintf(stdout, "we recv %d bytes data\n", length);
	} while (length >= toRead);
	return 0;
}

int discard (BsonBuffer &m_readBuffer) {
	if (!m_readBuffer.parseable()) {
		return 0;
	}

	if (m_readBuffer.parseFromBuffer(1) == -1) {
		fprintf(stdout, "parse bson buffer error\n");
		return -1;
	}

	//PlayerSession * ps = new PlayerSession;

	//m_res->getServletsDispatch()->dispatch(m_res, (PlayerSession*)this, m_readBuffer.protocol());

	//Message *msg = new Message;
	//msg->ps = (PlayerSession*)this; ////////////////error
	//msg->request = m_readBuffer.protocol(); ///////////copy once
	//globalQueue().push(msg);
	return 0;
}

}
}
