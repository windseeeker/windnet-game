#ifndef __PLAYER_SESSION_MANAGER_H__
#define __PLAYER_SESSION_MANAGER_H__

#include <boost/shared_ptr.hpp>

#include <map>

class PlayerSession;
class PlayerSessionManager {
public:
	PlayerSession *getOnlinePlayer(int roleId);
	PlayerSession *getPlayer(int roleId);

	void playerOffline(PlayerSession *ps);

private:
	std::map<int, PlayerSession*> m_onlinePlayers;
	std::map<int, PlayerSession*> m_cachePlayers; //cache player works as LRU
};

#endif
