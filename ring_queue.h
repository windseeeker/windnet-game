#ifndef __RING_QUEUE_H__
#define __RING_QUEUE_H__

#include <vector>

namespace Windnet {
class Message;
class RingQueue {
public:
	RingQueue();

	int push(Message *msg);
	Message* pop();

private:
	long long m_head;
	long long m_tail;
	std::vector<Message*> m_queue;
};

}
#endif
