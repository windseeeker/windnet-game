#include "timer.h"

#include <time.h>
#include <unistd.h>

namespace Windnet {
void TimerManager::addTimer(TimerItem *item) {
	std::list<TimerItem*>::iterator it = m_timers.begin();
	for (; it != m_timers.end(); ++it) {
		if (item->expireTime <= (*it)->expireTime) {
			m_timers.insert(it, item);
			break;
		}
	}
	m_timers.insert(it, item);
}

void TimerManager::run() {
	for (; ;) {
		time_t t = time(0);

		std::list<TimerItem*>::iterator it = m_timers.begin();
		if (it == m_timers.end() || (*it)->expireTime > t) {
			sleep(2);
		}
	}
}

}
