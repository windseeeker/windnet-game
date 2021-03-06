#ifndef __TASK_SERVLET_H__
#define __TASK_SERVLET_H__

#include "servlet.h"

class ServerResource;

namespace Windnet {
	namespace Net {
		class Socket;
	}
}

class PlayerSession;
class TaskServlet : public Servlet {
public:
	void setupServlet() {}
	bool doRequest(const std::string &type, ServerResource *res, const std::string &token,
				   PlayerSession *ps, Windnet::BSON::Object *request);

protected:
	bool doLoadTasks(ServerResource *res, const std::string &token,
					 PlayerSession *ps, Windnet::BSON::Object *request);

	bool doAcceptTask(ServerResource *res, const std::string &token,
					  PlayerSession *ps, Windnet::BSON::Object *request);

	bool doFinishTask(ServerResource *res, const std::string &token,
					  PlayerSession *ps, Windnet::BSON::Object *request);
};
#endif
