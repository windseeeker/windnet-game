#ifndef __SERVER_RESOURCE_H__
#define __SERVER_RESOURCE_H__

#include <boost/shared_ptr.hpp>

namespace Windnet {
class RingQueue;
namespace Mysql {
	class DBConnection;
	class DBConnectionPool;
}
}

class AccountManager;
class RoleManager;
class PlayerSessionManager;
class SceneManager;
class ServletDispatch;
class ServerManager;
class GameTemplateManager;

class ServerResource {
public:
	typedef boost::shared_ptr<ServerResource> ptr;

	ServerResource() {}
	void init();

	boost::shared_ptr<Windnet::Mysql::DBConnection> getDBConnection();

	boost::shared_ptr<Windnet::RingQueue> getQueue() { return m_serverQueue; }

	boost::shared_ptr<AccountManager> getAccountManager() { return m_accountMgr; }
	boost::shared_ptr<RoleManager> getRoleManager() { return m_roleManager; }
	boost::shared_ptr<SceneManager> getSceneManager() { return m_sceneManager; }
	boost::shared_ptr<ServletDispatch> getServletsDispatch() { return m_servletsDispatch; }

	boost::shared_ptr<ServerManager> getServerManager() { return m_serverManager; }
	PlayerSessionManager *getPlayerSessionManager() { return m_psm; }

	boost::shared_ptr<GameTemplateManager> getTemplateManager() { return m_templateManager; }

private:
	boost::shared_ptr<GameTemplateManager> m_templateManager;
	boost::shared_ptr<AccountManager> m_accountMgr;
	boost::shared_ptr<RoleManager> m_roleManager;
	boost::shared_ptr<SceneManager> m_sceneManager;

	PlayerSessionManager *m_psm;
	boost::shared_ptr<ServerManager> m_serverManager;

	boost::shared_ptr<ServletDispatch> m_servletsDispatch;
	boost::shared_ptr<Windnet::RingQueue>  m_serverQueue;
	boost::shared_ptr<Windnet::Mysql::DBConnectionPool> m_dbconnectionPool;
};

#endif
