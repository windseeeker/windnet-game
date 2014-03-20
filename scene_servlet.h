#ifndef __SCENE_SERVLET_H__
#define __SCENE_SERVLET_H__

#include "servlet.h"

class ServerResource;

class PlayerSession;
class SceneServlet : public Servlet {
public:
	void setupServlet() {}
	bool doRequest(const std::string &type, ServerResource *res, const std::string &token,
				   PlayerSession *ps, Windnet::BSON::Object *request);

protected:
	bool doEnterScene(ServerResource *res, const std::string &token,
					  PlayerSession *ps, Windnet::BSON::Object *request);

	bool doAvatarWalk(ServerResource *res, const std::string &token,
					  PlayerSession *ps, Windnet::BSON::Object *request);

	bool doEnterInstance(ServerResource *res, const std::string &token,
						 PlayerSession *ps, Windnet::BSON::Object *request);

	bool doNotifyMonsterDead(ServerResource *res, const std::string &token,
							 PlayerSession *ps, Windnet::BSON::Object *request);

	bool doLeaveInstanceScene(ServerResource *res, const std::string &token,
							  PlayerSession *ps, Windnet::BSON::Object *request);

};

#endif
