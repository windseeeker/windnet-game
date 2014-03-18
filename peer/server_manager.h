#ifndef __PEER_SERVER_MANAGER_H__
#define __PEER_SERVER_MANAGER_H__

#include <boost/shared_ptr.hpp>

#include <map>
#include <list>
#include <string>

namespace Windnet {
namespace Net {
	class Socket;
}
}

struct ServerInfo {
	typedef boost::shared_ptr<ServerInfo> ptr;
	enum {
		SERVER_STATE_NORMAL = 0,
		SERVER_STATE_BUSY = 1
	};

	int serverId;
	std::string serverName;
	std::string domain;
	std::string town;
	std::string line;
	int state;
	int serverState;
	unsigned short port;
};

class ServerManager {
public:
	typedef boost::shared_ptr<ServerManager> ptr;
	ServerManager();

	void getServers(const std::string &town,  std::list<ServerInfo::ptr>& list);
	void getAll(std::list<ServerInfo::ptr>& list);

	ServerInfo::ptr get(int serverId);
	ServerInfo::ptr getRecommandServer();

private:
	std::map<int, ServerInfo::ptr> m_idMap;
	std::map<std::string, std::list<ServerInfo::ptr> > m_servers;
};


#endif
