#ifndef __BUFFER_H__
#define __BUFFER_H__

#include <vector>

namespace Windnet {
namespace Net {
	class Socket;
}

class Buffer {
	friend class Windnet::Net::Socket;
public:
	static const unsigned int MAX_BUFFER_SIZE = 8 * 1024;
	Buffer();

	void reset();

	char *buffer() { return m_buffer; }

	void copyIn(const char *buf, int datalen);

	int readAvailable() const { return m_write - m_read; }
	int writeAvailable() { return 0; }

	int add(char *buf, int size);

	virtual bool parseable() { return false; }

protected:
	unsigned int m_read;
	unsigned int m_write;
	char m_buffer[MAX_BUFFER_SIZE];
};
}
#endif
