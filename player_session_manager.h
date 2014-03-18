#ifndef __PLAYER_SESSION_MANAGER_H__
#define __PLAYER_SESSION_MANAGER_H__

#include <boost/shared_ptr.hpp>

#include <map>

class PlayerSession;
class PlayerSessionManager {
public:
	typedef boost::shared_ptr<PlayerSessionManager> ptr;

	PlayerSession *getOnlinePlayer(int roleId);

private:
	std::map<int, PlayerSession*> m_onlinePlayers;
	std::map<int, PlayerSession*> m_cachePlayers;
};

#endif
