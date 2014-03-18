#ifndef __TIMER_H__
#define __TIMER_H__

#include <list>

namespace Windnet {
struct TimerItem {
	int expireTime;
	int sessionId;
};

class TimerManager {
public:
	void run();

	void addTimer(TimerItem* item);

private:
	std::list<TimerItem*> m_timers;
};

}

#endif
