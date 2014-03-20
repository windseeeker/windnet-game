#include "scene_servlet.h"

#include <stdio.h>

#include "account_manager.h"
#include "dataset/scene_template_manager.h"
#include "game_template_manager.h"
#include "role.h"
#include "server_resource.h"
#include "scene.h"
#include "scene_manager.h"
#include "socket.h"
#include "player_session.h"

using namespace Windnet;
using namespace Windnet::Net;
bool SceneServlet::doRequest(const std::string &type, ServerResource *res, const std::string &token,
							 PlayerSession *ps, BSON::Object *request) {
	if (type == "InLobby") {
		return doEnterScene(res, token, ps, request);
	} else if (type == "RunInLobby") {
		return doAvatarWalk(res, token, ps, request);
	} else if (type == "JoinPart") {
		return doEnterInstance(res, token, ps, request);
	} else if (type == "MonsterDead") {
		return doNotifyMonsterDead(res, token, ps, request);
	} else if (type == "PartLeave") {
		return doLeaveInstanceScene(res, token, ps, request);
	}

	fprintf(stdout, "Unknown protocol %s\n", type.c_str());
	fflush(stdout);
	return false;
}

bool SceneServlet::doEnterScene(ServerResource *res, const std::string &token,
								PlayerSession *ps,  BSON::Object *request) {
	int sceneId = BSON::getIntVal("SceneId", request);

	AccountInfo::ptr ai = res->getAccountManager()->getAccountInfoByToken(token);
	if (!ai || !ai->current) {
		printf("No such account info when enter scene\n");
		return true;
	}
	if (sceneId != 1) {
		printf("sceneId not 1 while enter scene\n");
		sceneId = 1;
	}
	SceneManager::ptr sm = res->getSceneManager();
	Dataset::SceneTemplateManager *stm = res->getTemplateManager()->getSceneTemplateManager();
	Dataset::SceneTemplate *st = stm->get(sceneId);
	if (!st) {
		printf("No such scene id %d ignoring\n", sceneId);
	}
	Scene::ptr scene = sm->get(sceneId);
	if (!scene) {
		printf("No such scene %d when enter scene\n", sceneId);
		return false;
	}
	Role *role = ps->role();
	role->scene(scene.get()); //////////
	scene->addPlayer(ai->current->roleId, ps);

	BSON::Object broadcast, body;
	BSON::setStringVal(broadcast, "Command", "OtherJoinLobby");
	BSON::setIntVal(body, "Id", ai->current->roleId);
	BSON::setStringVal(body, "Name", ai->current->roleName);
	BSON::setIntVal(body, "ProfessionId", ai->current->profession);
	BSON::setIntVal(body, "Pet", 1);
	BSON::setIntVal(body, "Mounts", 1);
	BSON::setObjectVal(broadcast, "Body", body);

	Buffer buf;
	BSON::serializeToBuffer(buf, broadcast);
	BsonBuffer::serializeToBuffer(buf);
	scene->broadcast(buf, ai->current->roleId);
	scene->sendSurrounding(ai->current->roleId, ps);
	return true;
}

bool SceneServlet::doAvatarWalk(ServerResource *res, const std::string &token,
								PlayerSession *ps, BSON::Object *request) {
	AccountInfo::ptr ai = res->getAccountManager()->getAccountInfoByToken(token);
    if (!ai || !ai->current) {
		printf("No such account info when enter scene\n");
		return true;
    }
	double posX = BSON::getDoubleVal("X", request);
	double posY = BSON::getDoubleVal("Y", request);
	Scene *scene = ps->role()->scene();
	if (!scene) {
		printf("Role %d has no scene when walk\n", ai->current->roleId);
		return false;
	}
	Scene::ptr scene2 = res->getSceneManager()->get(1);
	printf("doAvatarWalk, move to [%f, %f]\n", posX, posY);
	ai->current->posX = posX;
	ai->current->posY = posY;

	BSON::Object response, body;
	BSON::setStringVal(response, "Command", "OtherRunInLobby");
	BSON::setIntVal(body, "Id", ai->current->roleId);
	BSON::setDoubleVal(body, "X", posX);
	BSON::setDoubleVal(body, "Y", posY);
	BSON::setObjectVal(response, "Body", body);
	Buffer buf;
	BSON::serializeToBuffer(buf, response);
	BsonBuffer::serializeToBuffer(buf);
	scene->broadcast(buf, ai->current->roleId);
	return true;
}

bool SceneServlet::doEnterInstance(ServerResource *res, const std::string &token,
								   PlayerSession *ps, BSON::Object *request) {
	printf("Join part\n");
	Scene *scene = ps->role()->scene();
	if (!scene) {
		return false;
	}

	BSON::Object response, body;
	BSON::setStringVal(response, "Command", "JoinPart");
	BSON::setBoolVal(body, "Success", true);
	BSON::setObjectVal(response, "Body", body);

	Buffer buf;
	BSON::serializeToBuffer(buf, response);
	BsonBuffer::serializeToBuffer(buf);
	ps->sendData(buf);
	return true;
}

bool SceneServlet::doNotifyMonsterDead(ServerResource *res, const std::string &token,
									   PlayerSession *ps, BSON::Object *request) {
	int monsterId = BSON::getIntVal("Id", request);
	printf("notify monster dead, monId %d\n", monsterId);

	BSON::Object response, body;
	BSON::setStringVal(response, "Command", "MonsterDead");
	BSON::setBoolVal(body, "Success", true);
	BSON::setObjectVal(response, "Body", body);

    Buffer buf;
	BSON::serializeToBuffer(buf, response);
	BsonBuffer::serializeToBuffer(buf);
    ps->sendData(buf);
    return true;
}

bool SceneServlet::doLeaveInstanceScene(ServerResource *res, const std::string &token,
										PlayerSession *ps, BSON::Object *request) {
	printf("Do leave instance scene");

	BSON::Object response, body;
	BSON::setStringVal(response, "Command", "LeavePart");
	BSON::setBoolVal(body, "Success", true);
	BSON::setObjectVal(response, "Body", body);

	Buffer buf;
	BSON::serializeToBuffer(buf, response);
	BsonBuffer::serializeToBuffer(buf);
    ps->sendData(buf);
	return true;
}
