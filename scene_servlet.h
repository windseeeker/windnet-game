#ifndef __SCENE_SERVLET_H__
#define __SCENE_SERVLET_H__

#include "servlet.h"

class ServerResource;

class PlayerSession;
class SceneServlet : public Servlet {
public:
	void setupServlet() {}
	bool doRequest(const std::string &type, boost::shared_ptr<ServerResource> res, const std::string &token,
				   PlayerSession *ps, Windnet::BSON::Object *request);

protected:
	bool doEnterScene(boost::shared_ptr<ServerResource> res, const std::string &token,
					  PlayerSession *ps, Windnet::BSON::Object *request);

	bool doAvatarWalk(boost::shared_ptr<ServerResource> res, const std::string &token,
					  PlayerSession *ps, Windnet::BSON::Object *request);

	bool doEnterInstance(boost::shared_ptr<ServerResource> res, const std::string &token,
						 PlayerSession *ps, Windnet::BSON::Object *request);

	bool doNotifyMonsterDead(boost::shared_ptr<ServerResource> res, const std::string &token,
							 PlayerSession *ps, Windnet::BSON::Object *request);

	bool doLeaveInstanceScene(boost::shared_ptr<ServerResource> res, const std::string &token,
							  PlayerSession *ps, Windnet::BSON::Object *request);

};

#endif
