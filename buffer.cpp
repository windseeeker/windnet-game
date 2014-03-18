#include "buffer.h"

#include <assert.h>
#include <memory.h>

#include <iostream>

namespace Windnet {

Buffer::Buffer()
	:m_read(0)
	,m_write(0) {
}
void Buffer::reset() {
	m_write = m_read = 0;
}

void Buffer::copyIn(const char *buf, int size) { // just to move write pos
	if (buf != 0) {
		memcpy(m_buffer + m_write, buf, size);
	}
	m_write += size;
}

int Buffer::add(char *buf, int size) {
	if (m_write + size >= MAX_BUFFER_SIZE) {
		if (m_read == 0) {
			//no space to hold the data
			std::cerr << "No space to hold buffer data\n";
			return -1;
		}
		//try move data
		memmove(m_buffer, m_buffer + m_read, readAvailable());
		m_write -= m_read;
		m_read = 0;
	}
	if (m_write + size >= MAX_BUFFER_SIZE) {
		//no space to hold the data
		std::cerr << "No space to hold buffer data\n";
		return -1;
	}
	memcpy(m_buffer + m_write, buf, size);
    m_write += size;
    return m_write;
}

}
