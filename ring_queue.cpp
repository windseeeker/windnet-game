#include "ring_queue.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "message.h"
#include "log.h"
#include "spin_lock.h"

namespace Windnet {
RingQueue::RingQueue():m_head(0)
					  ,m_tail(0) {
	m_queue.resize(8096, 0);
}

int RingQueue::push(Message *msg) {
	long long idx = __sync_fetch_and_add(&m_tail, 1) % m_queue.size();
	m_queue[idx] = msg;
	//fprintf(stdout, "push message to queue %d\n", idx);
	//fflush(stdout);
	return 0;
}

Message *RingQueue::pop() {
	long long head = m_head % m_queue.size();
	//fprintf(stdout, "pop message from queue, idx%d\n", head);
	//fflush(stdout);
	if (m_queue[head] == NULL) {
		return NULL;
	}

	__sync_fetch_and_add(&m_head, 1);
	return m_queue[head];
	//return m_queue[m_head % m_queue.size()];
}
}

