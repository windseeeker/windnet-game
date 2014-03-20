#include "server_resource.h"

#include "account_manager.h"
#include "scene_manager.h"
#include "game_template_manager.h"
#include "role_manager.h"
#include "mysql/dbconnection.h"
#include "mysql/dbconnection_pool.h"
#include "player_session_manager.h"
#include "servlet.h"
#include "ring_queue.h"

#include "peer/server_manager.h"

using namespace Windnet;
//using namespace Windnet::Net;
using namespace Windnet::Mysql;

void ServerResource::init() {
	m_dbconnectionPool.reset(new DBConnectionPool("localhost", "root", "123456", "warrior", 10));

	m_serverQueue.reset(new RingQueue);
	m_servletsDispatch.reset(new ServletDispatch);
	m_servletsDispatch->init();

	m_serverManager.reset(new ServerManager);

	m_templateManager.reset(new GameTemplateManager);
	m_templateManager->init(); /*init all game template file */

	m_accountMgr.reset(new AccountManager);
	m_accountMgr->loadFromDB(getDBConnection());

	m_roleManager.reset(new RoleManager);

	m_sceneManager.reset(new SceneManager);
	m_sceneManager->initScenes(m_templateManager->getSceneTemplateManager());

	m_psm = new PlayerSessionManager;
}

DBConnection::ptr ServerResource::getDBConnection() {
	return m_dbconnectionPool->getDBConnection();
}
