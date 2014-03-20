#include "skill_servlet.h"

#include <stdio.h>

#include "account_manager.h"
#include "server_resource.h"
#include "socket.h"
#include "player_session.h"

using namespace Windnet;
using namespace Windnet::Net;

bool SkillServlet::doRequest(const std::string &type, ServerResource *res, const std::string &token,
							 PlayerSession *ps, BSON::Object *request) {
	if (type == "SkillInfo") {
		return doLoadSkills(res, token, ps, request);
	} else if (type == "UpSkill") {
		return doUpgradeSkill(res, token, ps, request);
	} else if (type == "EnableSkill") {
		return doUpgradeSkill(res, token, ps, request);
	} else if (type == "EquipSkill") {
		return doUpgradeSkill(res, token, ps, request);
	} else if (type == "UnEquipSkill") {
		return doUpgradeSkill(res, token, ps, request);
	}

	fprintf(stdout, "Unknown protocol %s\n", type.c_str());
	fflush(stdout);
	return false;
}

bool SkillServlet::doLoadSkills(ServerResource *res, const std::string &token,
								PlayerSession *ps,  BSON::Object *request) {

	BSON::Object response, body;
	BSON::Array array;
	BSON::setStringVal(response, "Command", "SkillInfo");
	BSON::setArrayVal(body, "Skills", array);

	BSON::setObjectVal(response, "Body", body);

	Buffer buf;
	BSON::serializeToBuffer(buf, response);
	BsonBuffer::serializeToBuffer(buf);
	ps->sendData(buf);
	return true;
}

bool SkillServlet::doUpgradeSkill(ServerResource *res, const std::string &token,
								  PlayerSession *ps, BSON::Object *request) {
	fprintf(stdout, "doUpgradeSkill\n");
	return true;
}
