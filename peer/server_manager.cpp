#include "server_manager.h"

ServerManager::ServerManager() {
	//just for test
	ServerInfo::ptr server(new ServerInfo);
	server->serverId = 2;
	server->serverName = "测试服务器1";
	server->domain = "172.16.5.26";
	server->town = "华北区";
	server->line = "电信";
	server->state = 0;
	server->port = 9000;
	server->serverState = 1;
	m_idMap[server->serverId] = server;

	server.reset(new ServerInfo);
	server->serverId = 3;
	server->serverName = "测试服务器2";
	server->domain = "172.16.5.26";
	server->town = "华男区";
	server->line = "网通";
	server->state = 0;
	server->port = 9001;
	server->serverState = 1;
	m_idMap[server->serverId] = server;
}

void ServerManager::getAll(std::list<ServerInfo::ptr>& list) {
	std::map<int, ServerInfo::ptr>::iterator it = m_idMap.begin();
	for (; it != m_idMap.end(); ++it) {
		list.push_back(it->second);
	}
}

void ServerManager::getServers(const std::string &town,  std::list<ServerInfo::ptr>& list) {
	std::map<std::string, std::list<ServerInfo::ptr> >::iterator it = m_servers.find(town);
	if (it == m_servers.end()) {
		return;
	}
	list = it->second;
}

ServerInfo::ptr ServerManager::getRecommandServer() {
	std::map<int, ServerInfo::ptr>::iterator it = m_idMap.begin();
	return it == m_idMap.end() ? ServerInfo::ptr() : it->second;
}

ServerInfo::ptr ServerManager::get(int serverId) {
	std::map<int, ServerInfo::ptr>::iterator it = m_idMap.find(serverId);
	return it == m_idMap.end() ? ServerInfo::ptr() : it->second;
}
