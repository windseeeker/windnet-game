#ifndef __THREAD_H__
#define __THREAD_H__


namespace Windnet {
class Thread {
public:
	virtual ~Thread() {}

	//virtual void *threadRoutine(void *) = 0;

	int work_thread_init();

private:
	//
};

class WorkThread : public Thread {
public:
	static void *threadRoutine(void *);

};
}

#endif
