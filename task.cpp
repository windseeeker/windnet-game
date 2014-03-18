#include "task.h"

#include "mysql/dbconnection.h"

using namespace Windnet;
using namespace Windnet::Mysql;

void Task::loadFromDB(DBConnection::ptr, int roleId) {

}

bool Task::taskFinished(int taskId) {
	std::set<int>::iterator it = m_finishedTask.find(taskId);
	return it == m_finishedTask.end();
}
