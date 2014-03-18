#ifndef __PLAYER_SESSION_H__
#define __PLAYER_SESSION_H__

#include "socket.h"

#include <boost/shared_ptr.hpp>

class ServerResource;

class Role;
class RoleInfo;
class PlayerSession : public Windnet::Net::Socket {
public:
    PlayerSession():Socket(-1)
		,m_role(NULL) {}

	Role *role() { return m_role; }
	bool loadRole(boost::shared_ptr<ServerResource> res, int roleId, boost::shared_ptr<RoleInfo> &ri);

	boost::shared_ptr<RoleInfo> roleInfo();

	void close();
	void playerOnline();
	void playerOffline();

private:
	Role *m_role;
};

#endif
