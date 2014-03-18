#include "role_servlet.h"

#include <stdio.h>

#include "account_manager.h"
#include "dataset/role_attribute_template.h"
#include "error_code.h"
#include "peer/server_manager.h"
#include "role.h"
#include "role_manager.h"
#include "server_resource.h"
#include "socket.h"
#include "player_session.h"

using namespace Windnet;
using namespace Windnet::Net;

void RoleServlet::setupServlet() {
}

bool RoleServlet::doRequest(const std::string &type, ServerResource::ptr res,
							const std::string &token, PlayerSession *ps, BSON::Object *request) {
	if (type == "CreateRole") {
		return doCreateRole(res, token, ps, request);
	} else if (type == "ValidataNick") {
		return doValidateNickName(res, token, ps, request);
	} else if (type == "LoadRoles") {
		return doLoadRoles(res, token, ps, request);
	} else if (type == "ChooseRole") {
		return doChooseRole(res, token, ps, request);
	} else if (type == "RemoveRole") {
		return doRemoveRole(res, token, ps, request);
	} else if (type == "RecoveryRole") {
		return doRecoveryRole(res, token, ps, request);
	}

	fprintf(stdout, "Unknown protocol %s\n", type.c_str());
	fflush(stdout);
	return false;
}

bool RoleServlet::doCreateRole(ServerResource::ptr res, const std::string &token,
							   PlayerSession *ps,  BSON::Object *request) {
	fprintf(stdout, "doCreateRole\n");
	fflush(stdout);

	if (token.empty()) {
		fprintf(stdout, "Token empty while createRole\n");
		return false;
	}
	std::string name = BSON::getStringVal("Name", request);
	int profession = BSON::getIntVal("ProfessId", request);
	if (name.empty()) {
		fprintf(stdout, "Role name is empty while createRole\n");
		return false;
	}

	AccountManager::ptr am = res->getAccountManager();
	AccountInfo::ptr ai = am->getAccountInfoByToken(token);
	if (!ai) {
		fprintf(stdout, "Can't get account by token %s\n\n", token.c_str());
		return false;
	}
	RoleInfo::ptr ri = am->getRoleInfo(name);
	int error = 0;
	do {
		if (ri) {
			fprintf(stdout, "Role %s already exist\n", name.c_str());
			break;
		}
		error = am->createRole(res->getDBConnection(), ai, name, profession);
	} while (0);

	BSON::Object response, body;
	BSON::setStringVal(response, "Command", "CreateRole");
	if (error == 0) {
		BSON::setBoolVal(body, "Success", true);
	} else {
		BSON::setBoolVal(body, "Error", 1);
		BSON::setIntVal(body, "ErrorMsg", error);
	}
	BSON::setObjectVal(response, "Body", body);

	Buffer buf;
	BSON::serializeToBuffer(buf, response);
	BsonBuffer::serializeToBuffer(buf);
	ps->sendData(buf);
	return true;
}

bool RoleServlet::doValidateNickName(ServerResource::ptr res, const std::string &token,
									 PlayerSession *ps,  BSON::Object *request) {
	printf("Validate nick name\n");

	std::string name = BSON::getStringVal("Name", request);
	AccountManager::ptr am = res->getAccountManager();
	RoleInfo::ptr roleInfo = am->getRoleInfo(name);

	BSON::Object response, body;
	BSON::setStringVal(response, "Command", "ValidataNick");
	if (roleInfo) {
		BSON::setBoolVal(body, "Error", true);
		BSON::setIntVal(body, "ErrorMsg", ErrorCode::LOGIN_ERROR_NICK_NAME_ALREADY_EXIST);
	} else {
		BSON::setBoolVal(body, "Success", true);
	}
	BSON::setObjectVal(response, "Body", body);

    Buffer buf;
	BSON::serializeToBuffer(buf, response);
	BsonBuffer::serializeToBuffer(buf);
    ps->sendData(buf);
	return true;
}

bool RoleServlet::doLoadRoles(ServerResource::ptr res, const std::string &token,
							  PlayerSession *ps,  BSON::Object *request) {
	fprintf(stdout, "doLoadRoles\n");

	int accountId = BSON::getIntVal("UserId", request);
	bool error = false;
	AccountManager::ptr am = res->getAccountManager();
	AccountInfo::ptr ai = am->getAccountInfo(accountId);

	do {
		if (token.empty()) {
			fprintf(stdout, "Empty token while load roles\n");
			break;
		}
		if (!ai) {
			fprintf(stdout, "No such account id %d while load roles\n", accountId);
			break;
		}
		if (ai->token != token) {
			fprintf(stdout, "Token doesn't match while load roles\n");
			break;
		}
		//error =  false;
	} while (0);

	BSON::Object response, body;
	BSON::Array array;
	BSON::setStringVal(response, "Command", "LoadRoles");
	if (error) {
		BSON::setBoolVal(body, "Error", true);
		goto _result;
	}
	for (std::map<int, RoleInfo::ptr>::iterator it = ai->roles.begin();
		 it != ai->roles.end(); ++it) {
		BSON::Object obj;
		BSON::setIntVal(obj, "Id", it->second->roleId);
		BSON::setStringVal(obj, "Name", it->second->roleName);
		BSON::setIntVal(obj, "Level", it->second->level);
		BSON::setIntVal(obj, "ProfessionId", it->second->profession);
		BSON::setIntVal(obj, "State", 0);                 //need further processing
		BSON::setIntVal(obj, "SurplusDay", 1000);         //need further processing
		BSON::Value val;
		val = obj;
		val.valueType = BSON_OBJECT;
		array.push_back(val);
	}
	BSON::setArrayVal(body, "Roles", array);;

_result:
	BSON::setObjectVal(response, "Body", body);

	Buffer buf;
	BSON::serializeToBuffer(buf, response);
	BsonBuffer::serializeToBuffer(buf);
	ps->sendData(buf);
	return true;
}

bool RoleServlet::doChooseRole(ServerResource::ptr res, const std::string &token,
							   PlayerSession *ps,  BSON::Object *request) {
	fprintf(stdout, "doChooseRole\n");
	if (token.empty()) {
		printf("Empty token while chooseRole\n");
		return false;
	}
	int roleId = BSON::getIntVal("RoleId", request);
	AccountManager::ptr am = res->getAccountManager();
	AccountInfo::ptr ai = am->getAccountInfoByToken(token);
	if (!ai) {
		printf("No such token %s while chooseRole\n", token.c_str());
		return true;
	}
	std::map<int, RoleInfo::ptr>::iterator it = ai->roles.find(roleId);
	if (it == ai->roles.end()) {
		printf("No such roleid %d while chooseRole\n", roleId);
		return true;
	}
	RoleInfo::ptr roleInfo = it->second;
	ai->current = roleInfo;
	ps->loadRole(res, ai->current->roleId, ai->current);

	Role *role = ps->role();
	//ps->roleInfo(ai->current); //////////////////////////////////////
	Dataset::RoleAttribute::ptr ra = role->attribute();

	BSON::Object response, body, roleInfoObj;
	BSON::setIntVal(roleInfoObj, "SceneId", 1); //////////////////Added by william
	BSON::setIntVal(roleInfoObj, "ProfessionId", role->profession());
	BSON::setIntVal(roleInfoObj, "Gold", role->gold());
	BSON::setIntVal(roleInfoObj, "Silver", role->money());
	BSON::setIntVal(roleInfoObj, "VIP", role->vipLevel());
	BSON::setIntVal(roleInfoObj, "GameState", 0); ///////////?????
	BSON::setIntVal(roleInfoObj, "Level", role->level());
	BSON::setIntVal(roleInfoObj, "Experience", 0);
	BSON::setIntVal(roleInfoObj, "MP", ra->hp);
	BSON::setIntVal(roleInfoObj, "HP", ra->mp);
	BSON::setIntVal(roleInfoObj, "RMB", 0);
	BSON::setStringVal(roleInfoObj, "Name", role->roleName());
	BSON::setObjectVal(body, "RoleInfo", roleInfoObj);

	BSON::Array equipObj, skillObj, petObj, mountObj;
	BSON::setArrayVal(body, "Equips", equipObj);
	BSON::setArrayVal(body, "Skills", skillObj);
	BSON::setArrayVal(body, "Pets", petObj);
	BSON::setArrayVal(body, "Mounts", mountObj);

	BSON::setStringVal(response, "Command", "ChooseRole");
	BSON::setObjectVal(response, "Body", body);

	Buffer buf;
	BSON::serializeToBuffer(buf, response);
	BsonBuffer::serializeToBuffer(buf);
	ps->sendData(buf);

	//Sync property
	BSON::Object attr, attrBody;
	BSON::setStringVal(attr, "Command", "SyncProperty");
	BSON::setIntVal(attrBody, "MinAttack", ra->minPhysicalAttack);
	BSON::setIntVal(attrBody, "MaxAttack", ra->maxPhysicalAttack);
	BSON::setIntVal(attrBody, "MinMagicAttack", ra->minMagicAttack);
	BSON::setIntVal(attrBody, "MaxMagicAttack", ra->maxMagicAttack);
	BSON::setIntVal(attrBody, "Defense", ra->defence);
	BSON::setIntVal(attrBody, "MagicDefense", ra->magicDefence);
	BSON::setIntVal(attrBody, "Hit", ra->hit);
	BSON::setIntVal(attrBody, "Dodge", ra->dodge);
	BSON::setIntVal(attrBody, "Crit", ra->crit);
	BSON::setIntVal(attrBody, "Toughness", ra->toughness);
	BSON::setIntVal(attrBody, "Move", ra->moveSpeed);
	BSON::setIntVal(attrBody, "AttackSpeed", ra->attackSpeed);
	BSON::setIntVal(attrBody, "HP", ra->maxHP);
	BSON::setIntVal(attrBody, "MP", ra->maxMP);
	BSON::setObjectVal(attr, "Body", attrBody);
	Buffer buf2;
	BSON::serializeToBuffer(buf2, attr);
	BsonBuffer::serializeToBuffer(buf2);
    ps->sendData(buf2);
	return true;
}

bool RoleServlet::doRemoveRole(ServerResource::ptr res, const std::string &token,
							   PlayerSession *ps,  BSON::Object *request) {
    fprintf(stdout, "doRemoveRole\n");
    return true;
}

bool RoleServlet::doRecoveryRole(ServerResource::ptr res, const std::string &token,
								 PlayerSession *ps,  BSON::Object *request) {
	fprintf(stdout, "doRecoveryRole\n");
	return true;
}
