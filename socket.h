#ifndef __SOCKET_H__
#define __SOCKET_H__

#include "bson_buffer.h"

class sockaddr_in;

class ServerResource;

namespace Windnet {
class Buffer;
class BsonBuffer;

namespace Net {
class IPAddress {

};

class IPAddress4 : public IPAddress {
public:

};

class Socket {
public:
    Socket(int val)
		:m_fd(val)
		,m_serviceType(SERVICE_GATE)
		,m_socketState(SOCKET_STATE_NORMAL) { }
	virtual ~Socket() {}

	enum {
		SOCKET_STATE_NORMAL = 0,
		SOCKET_STATE_NOT_USED = 1,
		SOCKET_STATE_ABONDONED = 2
	};
	enum {
		SERVICE_GATE = 0,
		SERVICE_SHORT = 1,
		SERVICE_LONG = 2
	};

	int fd() const { return m_fd; }
	void fd(int val) { m_fd = val; }

	short serviceType() const { return m_serviceType; }
	void serviceType(short val) { m_serviceType = val; }

	void setSockOpt(int level, int opt, int val);
	void nonblocking();

	Socket * accept();
	void bind(sockaddr_in *addr);
	void listen();

	virtual void close();

	int recvTimeOut(int millisecond) { return 0; }
	int sendTimeOnt(int millisecond) { return 0; }

	int write(const char *buffer, size_t bufflen);
	int read(BsonBuffer &buffer, int toRead = -1);

	BsonBuffer &readBuffer() { return m_readBuffer; }
	bool sendData(Buffer &buffer);

public:
	static Socket *createSocket();

private:
	int m_fd;
	char m_serviceType;
	char m_socketState;
	BsonBuffer m_readBuffer;
	Buffer m_writeBuffer;
};

}
}

#endif
