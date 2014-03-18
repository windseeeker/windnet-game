#include "task_template_manager.h"

#include "../assert.h"

#include "csv_reader.h"

namespace Dataset {
bool TaskTemplateManager::init() {
	CSVReader reader;
	if (!reader.loadFile("./data/task.csv")) {
		ASSERT(false);
	}
    do {
		TaskTemplate *task = new TaskTemplate;
		ASSERT(reader.Attribute("Task_id", task->taskId));
		ASSERT(reader.Attribute("Task_Type", task->taskType));
		ASSERT(reader.Attribute("Task_Exp", task->rewardExp));
		ASSERT(reader.Attribute("Task_Money", task->rewardMoney));

		m_tasks.insert(std::make_pair(task->taskId, task)); 
    }  while (reader.moveNext());

	return true;
}

TaskTemplate *TaskTemplateManager::get(int id) {
	std::map<int, TaskTemplate*>::iterator it = m_tasks.find(id);
	return it == m_tasks.end() ? NULL : it->second;
}
}
