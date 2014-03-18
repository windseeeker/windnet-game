#ifndef __TASK_TEMPLATE_MANAGER_H__
#define __TASK_TEMPLATE_MANAGER_H__

#include <map>
#include <vector>

namespace Dataset {
struct TaskTemplate {
	enum {
		TASK_TYPE_NPC_TALK  = 1,
		TASK_TYPE_KILL_MONSTER = 2,
		TASK_TYPE_COLLECT = 3,
		TASK_TYPE_POST = 4,
		TASK_TYPE_PASS_INSTANCE = 5,
		TASK_TYPE_ENHANCE_EQUIP = 6,
		TASK_TYPE_ENHANCE_MOUNT = 7,
		TASK_TYPE_ENHANCE_STAR = 8,
		TASK_TYPE_ENHANCE_PET = 9,
		TASK_TYPE_FORCE_VALUE = 10
	};
	int taskType;
	int taskId;
	int rewardExp;
	int rewardMoney;
	std::vector<std::pair<int, int> > taskCondition;
	std::vector<std::pair<int, int> > rewardItems;
	std::vector<int> preTask;
};

class TaskTemplateManager {
public:
	bool init();

	TaskTemplate *get(int id);

private:
	std::map<int, TaskTemplate*> m_tasks;
};
}

#endif
