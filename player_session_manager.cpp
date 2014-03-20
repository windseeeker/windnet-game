#include "player_session_manager.h"

#include "mysql/dbconnection.h"
#include "player_session.h"
#include "role.h"

PlayerSession *PlayerSessionManager::getOnlinePlayer(int roleId) {
	std::map<int, PlayerSession*>::iterator it = m_onlinePlayers.find(roleId);
	return it == m_onlinePlayers.end() ? NULL : it->second;
}

PlayerSession *PlayerSessionManager::getPlayer(int roleId) {
	std::map<int, PlayerSession*>::iterator it = m_onlinePlayers.find(roleId);
	if (it != m_onlinePlayers.end()) {
		return it->second;
	}

	it = m_cachePlayers.find(roleId);
	if (it != m_cachePlayers.end()) {
		return it->second;
	}
	return NULL;
}

void PlayerSessionManager::playerOffline(PlayerSession *ps) {
	if (!ps->role()) {
		return;
	}
	m_onlinePlayers.erase(ps->role()->id());
	m_cachePlayers.insert(std::make_pair(ps->role()->id(), ps));
}
