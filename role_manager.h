#ifndef __ROLE_MANAGER_H__
#define __ROLE_MANAGER_H__

#include <boost/shared_ptr.hpp>

#include <map>
#include <set>

namespace Windnet {
	namespace Mysql {
		class DBConnection;
	}
}

struct RoleInfo;
class RoleManager {
public:
	typedef boost::shared_ptr<RoleManager> ptr;

	void loadFromDB(boost::shared_ptr<Windnet::Mysql::DBConnection> conn);
	void flushToDB(boost::shared_ptr<Windnet::Mysql::DBConnection> conn);

	void flushToCache() {}
	void flushToDB() {}

	int createRole(boost::shared_ptr<Windnet::Mysql::DBConnection> conn,
				   const std::string &name, const std::string &paswd, const std::string& deviceId);

	boost::shared_ptr<RoleInfo> getRoleInfo(const std::string &name);

private:
	std::map<std::string, boost::shared_ptr<RoleInfo> > m_nameMap;
	std::map<int, boost::shared_ptr<RoleInfo> > m_idMap;
};

#endif
