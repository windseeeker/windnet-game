#ifndef __SKILL_SERVLET_H__
#define __SKILL_SERVLET_H__

#include "servlet.h"

class ServerResource;

class PlayerSession;
class SkillServlet : public Servlet {
public:
	void setupServlet() {}
	bool doRequest(const std::string &type, ServerResource *res, const std::string &token,
				   PlayerSession *ps, Windnet::BSON::Object *request);

protected:
	bool doLoadSkills(ServerResource *res, const std::string &token,
					  PlayerSession *ps, Windnet::BSON::Object *request);

	bool doUpgradeSkill(ServerResource *res, const std::string &token,
						PlayerSession *ps, Windnet::BSON::Object *request);

};
#endif
