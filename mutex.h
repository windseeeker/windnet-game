#ifndef __MUTEX_LOCK_H__
#define __MUTEX_LOCK_H__

#include <pthread.h>

namespace Windnet {
class ScopedLock;
class Mutex {
friend class ScopedLock;
public:
    Mutex() {
		thread_mutex_init(&_mutex, NULL);
	}
	~Mutex() {
		pthread_mutex_destroy(&_mutex);
	}

private:
	pthread_mutex_t _mutex;
};

class ScopedLock {
public:
    ScopedLock(Mutex &mutex)
		:_lock(mutex) {
		pthread_mutex_lock(&_lock._mutex);
	}
	~ScopedLock() {
		pthread_mutex_unlock(&_lock._mutex);
	}

private:
	Mutex &_lock;
};
}

#endif
