#ifndef __TASK_H__
#define __TASK_H__

#include <boost/shared_ptr.hpp>

#include <set>


namespace Windnet {
	namespace Mysql {
		class DBConnection;
	}
}

class Task {
public:
	typedef boost::shared_ptr<Task> ptr;
	enum {
		TASK_STATE_NOT_ACCEPTED = 0,
		TASK_STATE_ACCEPTED = 1,
		TASK_STATE_FINISHABLE = 2,
		TASK_STATE_FINISHED = 3
	};

	void loadFromDB(boost::shared_ptr<Windnet::Mysql::DBConnection>, int roleId);

	bool taskFinished(int taskId);

private:
	std::set<int> m_finishedTask;
};

#endif
