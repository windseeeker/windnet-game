#include "servlet.h"

#include "message.h"
#include "server_resource.h"
#include "player_session.h"
#include "session.h"

#include "login_servlet.h"
#include "role_servlet.h"
#include "gem_servlet.h"
#include "item_servlet.h"
#include "scene_servlet.h"
#include "skill_servlet.h"
#include "task_servlet.h"
#include "soul_item_servlet.h"

#include "peer/server_manager.h"

using namespace Windnet;
using namespace Windnet::Net;

void ServletDispatch::init() {
	Servlet::ptr servlet(new LoginServlet);
	m_servlets.insert(std::make_pair("Register", servlet));
	m_servlets.insert(std::make_pair("ValidataName", servlet));
	m_servlets.insert(std::make_pair("Login", servlet));
	m_servlets.insert(std::make_pair("AutoLogin", servlet));
	m_servlets.insert(std::make_pair("LoadServers", servlet));
	m_servlets.insert(std::make_pair("ChooseServer", servlet));
	m_servlets.insert(std::make_pair("LoadTowns", servlet));
	m_servlets.insert(std::make_pair("GetRecommandServers", servlet));
	m_servlets.insert(std::make_pair("GetRecommandServer", servlet));
	m_servlets.insert(std::make_pair("GetSimpleServer", servlet));

	servlet.reset(new RoleServlet);
	m_servlets.insert(std::make_pair("CreateRole", servlet));
	m_servlets.insert(std::make_pair("ValidataNick", servlet));
    m_servlets.insert(std::make_pair("LoadRoles", servlet));
    m_servlets.insert(std::make_pair("ChooseRole", servlet));
	m_servlets.insert(std::make_pair("RemoveRole", servlet));
	m_servlets.insert(std::make_pair("RecoveryRole", servlet));

	servlet.reset(new SceneServlet);
	m_servlets.insert(std::make_pair("InLobby", servlet));
	m_servlets.insert(std::make_pair("RunInLobby", servlet));
	m_servlets.insert(std::make_pair("JoinPart", servlet));
	m_servlets.insert(std::make_pair("MonsterDead", servlet));
	m_servlets.insert(std::make_pair("PartLeave", servlet));

	servlet.reset(new TaskServlet);
	m_servlets.insert(std::make_pair("LoadTasks", servlet));
	m_servlets.insert(std::make_pair("AcceptTask", servlet));
	m_servlets.insert(std::make_pair("FinishTask", servlet));

	servlet.reset(new ItemServlet);
	m_servlets.insert(std::make_pair("OpenPackage", servlet));
	m_servlets.insert(std::make_pair("AppendPackage", servlet));
	m_servlets.insert(std::make_pair("AbandonProps", servlet));
	m_servlets.insert(std::make_pair("SortPackage", servlet));
	m_servlets.insert(std::make_pair("BuyPropFromBodyShop", servlet));
	m_servlets.insert(std::make_pair("InBody", servlet));
	m_servlets.insert(std::make_pair("GetBody", servlet));
	m_servlets.insert(std::make_pair("OutBody", servlet));
	m_servlets.insert(std::make_pair("StrengProps", servlet));
	m_servlets.insert(std::make_pair("MoveProps", servlet));
	m_servlets.insert(std::make_pair("SaleProps", servlet));
	m_servlets.insert(std::make_pair("GetEquipGem", servlet));
	m_servlets.insert(std::make_pair("PrevStrengProps", servlet));

	servlet.reset(new GemServlet);
    m_servlets.insert(std::make_pair("UpGem", servlet));
    m_servlets.insert(std::make_pair("DownGem", servlet));
    m_servlets.insert(std::make_pair("StrengGem", servlet));

	servlet.reset(new SkillServlet);
	m_servlets.insert(std::make_pair("SkillInfo", servlet));
    m_servlets.insert(std::make_pair("UpSkill", servlet));
    m_servlets.insert(std::make_pair("EnableSkill", servlet));
	m_servlets.insert(std::make_pair("EquipSkill", servlet));
	m_servlets.insert(std::make_pair("UnEquipSkill", servlet));

	servlet.reset(new SoulItemServlet);
	m_servlets.insert(std::make_pair("SoulMetaInfo", servlet));
	m_servlets.insert(std::make_pair("LotterySoulMeta", servlet));
	m_servlets.insert(std::make_pair("PacketSoulMeta", servlet));
	m_servlets.insert(std::make_pair("SwallowQuickly", servlet));
	m_servlets.insert(std::make_pair("SwallowOne", servlet));
	m_servlets.insert(std::make_pair("SoulMetaMove", servlet));
	m_servlets.insert(std::make_pair("BuySoulMeta", servlet));
	m_servlets.insert(std::make_pair("EquipSoulMeta", servlet));
	m_servlets.insert(std::make_pair("EquipSoulMetaInfo", servlet));
}

bool ServletDispatch::dispatchMessage(ServerResource::ptr res, Message *msg) {
	fprintf(stdout, "Dispatch message\n");
	std::string proto = BSON::getStringVal("Command", &msg->request);
	std::string token = BSON::getStringVal("Token", &msg->request);
	if (msg->ps->serviceType() != Socket::SERVICE_LONG && token.empty()) {
		//fprintf(stdout, "Fatal error, non-long service got empty token\n");
		//return false;
	}
	fprintf(stdout, "Command : %s, service %d \n", proto.c_str(), (int)msg->ps->serviceType());
	std::map<std::string, Servlet::ptr>::iterator it = m_servlets.find(proto);
	if (it == m_servlets.end()) {
		fprintf(stdout, "Can't find protocol %s\n", proto.c_str());
		return false;
	}
	BSON::Object body = BSON::getObjectVal("Body", &msg->request);
	std::string name = BSON::getStringVal("Name", &body);
	bool ret;
	try {
		ret = it->second->doRequest(proto, res, token, msg->ps, (BSON::Object*)&body);
	} catch (...) {
		ret = false;
		printf("EEEEEEEEEEEEXCEPION \n");
	}
	delete msg; /////////////TODO
	return ret;
}

bool ServletDispatch::dispatch(ServerResource::ptr res, PlayerSession *ps, BSON::Object &request) {
	std::string proto = BSON::getStringVal("Command", &request);
	std::string token = BSON::getStringVal("Token", &request);
	fprintf(stdout, "Command : %s, service \n", proto.c_str());//, (int)ps->serviceType());
	std::map<std::string, Servlet::ptr>::iterator it = m_servlets.find(proto);
	if (it == m_servlets.end()) {
		fprintf(stdout, "Can't find protocol %s\n", proto.c_str());
		return false;
	}
	BSON::Object body = BSON::getObjectVal("Body", &request);
	try {
		return it->second->doRequest(proto, res, token, ps, (BSON::Object*)&body);
	} catch (...) {
		printf("We catch exception while dispose reques\n");
		return false;
	}
}
