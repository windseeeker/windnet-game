#include "login_servlet.h"

#include <boost/lexical_cast.hpp>

#include <stdio.h>

#include "account_manager.h"
#include "error_code.h"
#include "peer/server_manager.h"
#include "server_resource.h"
#include "socket.h"
#include "player_session.h"

using namespace Windnet;
using namespace Windnet::Net;

void LoginServlet::setupServlet() {
}

bool LoginServlet::doRequest(const std::string &type, ServerResource *res,
							 const std::string &token, PlayerSession *ps, BSON::Object *request) {
	if (type == "Register") {
		return doRegister(res, token, ps, request);
	} else if (type == "ValidataName") {
		return doValidateName(res, token, ps, request);
	} else if (type == "Login") {
		return doLogin(res, token, ps, request);
	} else if (type == "AutoLogin") {
		return doAutoLogin(res, token, ps, request);
	} else if (type == "LoadServers") {
		return doLoadServers(res, token, ps, request);
	} else if (type == "ChooseServer") {
		return doChooseServer(res, token, ps, request);
	} else if (type == "LoadTowns") {
		return doLoadServerTowns(res, token, ps, request);
	} else if (type == "GetRecommandServers") {
		return doGetRecommandServers(res, token, ps, request);
	} else if (type == "GetSimpleServer") {
		return doGetServerInfo(res, token, ps, request);
	} else if (type == "GetRecommandServer") {
		return doGetCurrentRecommandSrv(res, token, ps, request);
	}
	fprintf(stdout, "Unknown protocol %s\n", type.c_str());
	fflush(stdout);
	return false;
}

bool LoginServlet::doRegister(ServerResource *res, const std::string &token,
							  PlayerSession *ps,  BSON::Object *request) {
	std::string name = BSON::getStringVal("Name", request);
	std::string passwd = BSON::getStringVal("Pwd", request);
	//std::string deviceId = request->getStringValue("DeviceId");

	AccountManager::ptr am = res->getAccountManager();
	int error = ErrorCode::ERROR_CODE_NO_ERROR;
	AccountInfo::ptr ai;
	do {
		if (name.empty() || passwd.empty()) {
			fprintf(stdout, "Register name or passwd empty while registering\n");
			return false;
		}
		ai = am->getAccountInfo(name);
		if (ai) {
			error = ErrorCode::REGISTER_ERROR_NAME_ALREADY_EXIST;
			fprintf(stdout, "Account %s already exist\n", name.c_str());
			break;
		}
		error = am->registerAccount(res->getDBConnection(), name, passwd);
		if (error == 0) {
			error = ErrorCode::ERROR_CODE_NO_ERROR;
		}
	} while (0);

	BSON::Object response, body;
	BSON::setStringVal(response, "Command", "Register");
	if (error == ErrorCode::ERROR_CODE_NO_ERROR) {
		BSON::setBoolVal(body, "Success", true);
	} else {
		BSON::setBoolVal(body, "Error", true);
		BSON::setIntVal(body, "ErrorMsg", error);
	}
	BSON::setObjectVal(response, "Body", body);

	Buffer buf;
	BSON::serializeToBuffer(buf, response);
	BsonBuffer::serializeToBuffer(buf);
	ps->sendData(buf);
	return true;
}

bool LoginServlet::doValidateName(ServerResource *res, const std::string &token,
								  PlayerSession *ps,  BSON::Object *request) {
	std::string name = BSON::getStringVal("Name", request);
	if (name.empty()) {
		fprintf(stdout, "empty name while validating\n");
		return false;
	}
	AccountManager::ptr am = res->getAccountManager();
	AccountInfo::ptr ai = am->getAccountInfo(name);

	BSON::Object response, body;
	if (!ai) {
		BSON::setBoolVal(body, "Success", true);
	} else {
		BSON::setBoolVal(body, "Error", true);
		BSON::setIntVal(body, "ErrorMsg", ErrorCode::REGISTER_ERROR_NAME_ALREADY_EXIST);
	}
	BSON::setStringVal(response, "Command", "ValidataName");
	BSON::setObjectVal(response, "Body", body);

	Buffer buf;
	BSON::serializeToBuffer(buf, response);
	BsonBuffer::serializeToBuffer(buf);
	ps->sendData(buf);
	return true;
}

bool LoginServlet::doLogin(ServerResource *res, const std::string &token,
						   PlayerSession *ps, BSON::Object *request) {
	std::string loginType = BSON::getStringVal("Type", request);
	if (loginType == "auto") {
		return doAutoLogin(res, token, ps, request);
    }
	if (loginType != "system") {
		fprintf(stdout, "Now we only support system login\n");
		return false;
	}
	//TODO  This type is not consistant
	int serverId;
	try {
		std::string str = BSON::getStringVal("ServerId", request);
		serverId = boost::lexical_cast<int>(str);
	} catch (...) {
		serverId = BSON::getIntVal("ServerId", request);
	}

	std::string name = BSON::getStringVal("Name", request);
	std::string passwd = BSON::getStringVal("Pwd", request);
	std::string deviceId = BSON::getStringVal("DeviceId", request);
	std::string deviceMode = BSON::getStringVal("DeviceMode", request);
	std::string screenSize =  BSON::getStringVal("ScreenSize", request);

	int error = ErrorCode::ERROR_CODE_NO_ERROR;
	AccountManager::ptr am = res->getAccountManager();
	AccountInfo::ptr ai = am->getAccountInfo(name);
	do {
		if (!ai) {
			fprintf(stdout, "Login error, no such account name %s\n", name.c_str());
			error = ErrorCode::LOGIN_ERROR_ACCOUNT_NON_EXIST;
			break;
		}
		if (ai->passwd != passwd) {
			fprintf(stdout, "Login error, user %s incorrent password \n", name.c_str());
			error = ErrorCode::LOGIN_ERROR_NAME_PASSWD_ERROR;
			break;
		}
		ServerInfo::ptr server = res->getServerManager()->get(serverId);
		if (!server) {
			fprintf(stdout, "No such serverId %d while logining\n", serverId);
			error = ErrorCode::LOGIN_ERROR_SERVER_NOT_SELECTED;
			break;
		}
	} while (0);

	BSON::Object response, body;
	if (error  == ErrorCode::ERROR_CODE_NO_ERROR) {
		BSON::setStringVal(body, "Token", ai->token);
		BSON::setIntVal(body, "UserId", ai->accountId);
		BSON::setStringVal(body, "Domain", "172.16.5.26");
		BSON::setIntVal(body, "Port", 20001);
		BSON::setStringVal(body, "SysOpenId", ai->sysOpenId);
		BSON::setStringVal(body, "IMAddress", "172.16.5.26");
		BSON::setStringVal(body, "MapAddress", "172.16.5.26");
		BSON::setStringVal(body, "PanelAddress", "172.16.5.26");
	} else {
		BSON::setBoolVal(body, "Error", true);
		BSON::setIntVal(body, "ErrorMsg", error);
	}
	BSON::setStringVal(response, "Command", "Login");
	BSON::setObjectVal(response, "Body", body);

    Buffer buf;
	BSON::serializeToBuffer(buf, response);
	BsonBuffer::serializeToBuffer(buf);
    ps->sendData(buf);
	return true;
}

bool LoginServlet::doAutoLogin(ServerResource *res, const std::string &token,
							   PlayerSession *ps,  BSON::Object *request) {
	AccountManager::ptr am = res->getAccountManager();
	std::string name = BSON::getStringVal("Name", request);
	std::string sysOpenId = BSON::getStringVal("SysOpenId", request);

	AccountInfo::ptr ai;
	bool ret = false;
	do {
		if (name.empty() || sysOpenId.empty()) {
			fprintf(stdout, "name or sysOpenId is empty while autologining\n");
			break;
		}
		ai = am->getAccountInfo(name);
		if (!ai) {
			fprintf(stdout, "No such account %s while autologin\n", name.c_str());
			break;
		}
		if (ai->sysOpenId != sysOpenId) {
			fprintf(stdout, "Account %s sysOpenId doesn't match while autologin\n", name.c_str());
			break;
		}
		ret = true;
	} while (0);

	BSON::Object response, body;
	BSON::setStringVal(response, "Command", "AutoLogin");
	if (!ret) {
		BSON::setBoolVal(body, "Error", true);
	} else {
		BSON::setStringVal(body, "Token", ai->token);
		BSON::setIntVal(body, "UserId", ai->accountId);
		BSON::setStringVal(body, "Domain", "172.16.5.26");
		BSON::setIntVal(body, "Port", 20000);
		BSON::setStringVal(body, "SysOpenId", ai->sysOpenId);
		BSON::setStringVal(body, "IMAddress", "172.16.5.26");
		BSON::setStringVal(body, "MapAddress", "172.16.5.26");
		BSON::setStringVal(body, "PanelAddress", "172.16.5.26");
	}
	BSON::setObjectVal(response, "Body", body);

	Buffer buf;
	BSON::serializeToBuffer(buf, response);
	BsonBuffer::serializeToBuffer(buf);
    ps->sendData(buf);
	return true;
}

bool LoginServlet::doLoadServerTowns(ServerResource *res, const std::string &token,
									 PlayerSession *ps, BSON::Object *request) {
	BSON::Object response, body;
	BSON::Array array;
	BSON::setStringVal(response, "Command", "LoadTowns");

	std::list<ServerInfo::ptr> servers;
	res->getServerManager()->getAll(servers);
	for (std::list<ServerInfo::ptr>::iterator it = servers.begin();
		 it != servers.end(); ++it) {
		BSON::Object obj;
		BSON::setStringVal(obj, "Name", (*it)->town);
		BSON::Value val;
		val = obj;
		val.valueType = BSON_OBJECT;
		array.push_back(val);
	}
	BSON::setArrayVal(body, "Towns", array);;
	BSON::setObjectVal(response, "Body", body);

	Buffer buf;
	BSON::serializeToBuffer(buf, response);
	BsonBuffer::serializeToBuffer(buf);
	ps->sendData(buf);
	return true;
}

bool LoginServlet::doLoadServers(ServerResource *res, const std::string &token,
								 PlayerSession *ps, BSON::Object *request) {
	BSON::Object response, body;
	BSON::Array array;

	BSON::setStringVal(response, "Command", "LoadServers");
	std::list<ServerInfo::ptr> servers;
	res->getServerManager()->getAll(servers);
	for (std::list<ServerInfo::ptr>::iterator it = servers.begin();
		 it != servers.end(); ++it) {
		BSON::Object obj;
		BSON::setIntVal(obj, "Id", (*it)->serverId);
		BSON::setStringVal(obj, "Name", (*it)->serverName);
		BSON::setStringVal(obj, "Town", (*it)->town);
		BSON::setStringVal(obj, "Line", (*it)->line);
		BSON::setIntVal(obj, "State", (*it)->state);
		BSON::setIntVal(obj, "ServerState", (*it)->serverState);
		BSON::Value val;
		val = obj;
		val.valueType = BSON_OBJECT;
		array.push_back(val);
	}
	BSON::setArrayVal(body, "Servers", array);
	BSON::setObjectVal(response, "Body", body);

	Buffer buf;
	BSON::serializeToBuffer(buf, response);
	BsonBuffer::serializeToBuffer(buf);
    ps->sendData(buf);
	return true;
}

bool LoginServlet::doGetCurrentRecommandSrv(ServerResource *res, const std::string &token,
											PlayerSession *ps, BSON::Object *request) {
	BSON::Object response, body;
	BSON::setStringVal(response, "Command", "GetRecommandServer");

	ServerInfo::ptr si = res->getServerManager()->getRecommandServer();
	if (!si) {
		printf("can't get current recommand server ");
		return true;
	}
	BSON::setIntVal(body, "Id", si->serverId);
	BSON::setStringVal(body, "Name", si->serverName);
	BSON::setObjectVal(response, "Body", body);

	Buffer buf;
	BSON::serializeToBuffer(buf, response);
	BsonBuffer::serializeToBuffer(buf);
	ps->sendData(buf);
	return true;
}

bool LoginServlet::doGetRecommandServers(ServerResource *res, const std::string &token,
										 PlayerSession *ps, BSON::Object *request) {
	BSON::Object response, body;
	BSON::Array array;
	BSON::setStringVal(response, "Command", "GetRecommandServers");

	std::list<ServerInfo::ptr> servers;
    res->getServerManager()->getAll(servers);
    for (std::list<ServerInfo::ptr>::iterator it = servers.begin();
         it != servers.end(); ++it) {
		BSON::Object obj;
		BSON::setIntVal(obj, "Id", (*it)->serverId);
		BSON::setStringVal(obj, "Name", (*it)->serverName);
		BSON::setStringVal(obj, "Town", (*it)->town);
		BSON::setStringVal(obj, "Line", (*it)->line);
		BSON::setIntVal(obj, "State", (*it)->state);
		BSON::setIntVal(obj, "ServerState", (*it)->serverState);
		BSON::Value val;
        val = obj;
        val.valueType = BSON_OBJECT;
		array.push_back(val);
	}
	BSON::setArrayVal(body, "Servers", array);
	BSON::setObjectVal(response, "Body", body);

    Buffer buf;
	BSON::serializeToBuffer(buf, response);
	BsonBuffer::serializeToBuffer(buf);
    ps->sendData(buf);
	return true;
}

bool LoginServlet::doGetServerInfo(ServerResource *res, const std::string &token,
								   PlayerSession *ps, BSON::Object *request) {
	int serverId = BSON::getIntVal("Id", request);
	fprintf(stdout, "DDDDDDDDDDDDDDDDDDDDDDDDOOOOOOOOOOOOOOOOOOOOOO get simple server %d \n", serverId);

	ServerManager::ptr sm = res->getServerManager();
	ServerInfo::ptr serverInfo = sm->get(serverId);

	BSON::Object response, body;
	BSON::setStringVal(response, "Command", "GetSimpleServer");
	if (!serverInfo) {
		BSON::setBoolVal(body, "Error", true);
		BSON::setIntVal(body, "ErrorMsg", 104);
	} else {
		BSON::setIntVal(body, "Id", serverId);
		BSON::setStringVal(body, "Name", serverInfo->serverName);
		BSON::setStringVal(body, "Town", serverInfo->town);
		BSON::setStringVal(body, "Line", serverInfo->line);
		BSON::setIntVal(body, "ServerState", serverInfo->serverState);
		BSON::setIntVal(body, "ServerState", serverInfo->state);
	}
	BSON::setObjectVal(response, "Body", body);

	Buffer buf;
	BSON::serializeToBuffer(buf, response);
	BsonBuffer::serializeToBuffer(buf);
	ps->sendData(buf);
	return true;
}

bool LoginServlet::doChooseServer(ServerResource *res, const std::string &token,
								  PlayerSession *ps, BSON::Object *request) {
	int serverId = BSON::getIntVal("Id", request);
	//int userId = request->getIntValue("UserId");

	BSON::Object response, body;
	BSON::setStringVal(response, "Command", "ChooseServer");

	ServerInfo::ptr serverInfo = res->getServerManager()->get(serverId);
	if (!serverInfo) {
		fprintf(stdout, "doChooseServer, load server info\n");
		return false;
	}
	BSON::setStringVal(body, "Domain", serverInfo->domain);
	BSON::setIntVal(body, "Port", serverInfo->port);
	BSON::setObjectVal(response, "Body", body);

    Buffer buf;
	BSON::serializeToBuffer(buf, response);
	BsonBuffer::serializeToBuffer(buf);
	ps->sendData(buf);
	return true;
}
