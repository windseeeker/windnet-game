#include "task_servlet.h"

#include <stdio.h>

#include "account_manager.h"
#include "dataset/task_template_manager.h"
#include "game_template_manager.h"
#include "role.h"
#include "task.h"
#include "server_resource.h"
#include "socket.h"
#include "player_session.h"

using namespace Windnet;
using namespace Windnet::Net;

bool TaskServlet::doRequest(const std::string &type, ServerResource::ptr res, const std::string &token,
							PlayerSession *ps, BSON::Object *request) {
	if (type == "LoadTasks") {
		return doLoadTasks(res, token, ps, request);
	} else if (type == "AcceptTask") {
		return doAcceptTask(res, token, ps, request);
	} else if (type == "FinishTask") {
		return doFinishTask(res, token, ps, request);
	}

	fprintf(stdout, "Unknown protocol %s\n", type.c_str());
	fflush(stdout);
	return false;
}

bool TaskServlet::doLoadTasks(ServerResource::ptr res, const std::string &token,
							  PlayerSession *ps,  BSON::Object *request) {
	BSON::Object response, body;

	return true;
}

bool TaskServlet::doAcceptTask(ServerResource::ptr res, const std::string &token,
							   PlayerSession *ps, BSON::Object *request) {
	int taskId = BSON::getIntVal("TaskId", request);
	fprintf(stdout, "doAcceptTask %d \n", taskId);

	Dataset::TaskTemplateManager *ttm = res->getTemplateManager()->getTaskTemplateManager();
	Dataset::TaskTemplate *taskTemp = ttm->get(taskId);
	if (!taskTemp) {
		printf("No such task id %d\n", taskId);
		return true;
	}
	bool error = false;
	Task *task = ps->role()->task();
	for (size_t i = 0; i < taskTemp->preTask.size(); ++i) {
		if (!task->taskFinished(taskTemp->preTask[i])) {
			error = true;
			break;
		}
	}
	BSON::Object response, body;
	BSON::setStringVal(response, "Command", "AcceptTask");
	if (error) {
		BSON::setBoolVal(body, "Error", true);
		BSON::setIntVal(body, "ErrorMsg", error);
	} else {
		BSON::setIntVal(body, "TaskId", taskId);
		BSON::setIntVal(body, "TaskUniqueId", taskId + 1);
	}
	BSON::setObjectVal(response, "Body", body);

	Buffer buf;
	BSON::serializeToBuffer(buf, response);
	BsonBuffer::serializeToBuffer(buf);
    ps->sendData(buf);
	return true;
}

bool TaskServlet::doFinishTask(ServerResource::ptr res, const std::string &token,
							   PlayerSession *ps,  BSON::Object *request) {
	int taskId = BSON::getIntVal("TaskUniqueId", request);

	fprintf(stdout, "doFiishTask %d\n", taskId);
	return true;
}
