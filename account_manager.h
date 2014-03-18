#ifndef __ACCOUNT_MANAGER_H__
#define __ACCOUNT_MANAGER_H__

#include <boost/shared_ptr.hpp>

#include <map>
#include <set>
#include <string>

namespace Windnet {
	namespace Mysql {
		class DBConnection;
	}
}

struct RoleInfo {
	typedef boost::shared_ptr<RoleInfo> ptr;

	int roleId;
	short level;
	char profession;
	unsigned int registerTime;
	std::string roleName;
	std::string token;
	double posX, posY;
};

struct AccountInfo {
	typedef boost::shared_ptr<AccountInfo> ptr;

	int accountId;
	short accountState;
	unsigned int registerTime;
	std::string accountName;
	std::string passwd;
	std::string token;
	std::string deviceId;
	std::string sysOpenId;
	RoleInfo::ptr current;
	std::map<int, RoleInfo::ptr> roles;
};

class AccountManager {
public:
	typedef boost::shared_ptr<AccountManager> ptr;

	void loadFromDB(boost::shared_ptr<Windnet::Mysql::DBConnection> conn);
	void flushToDB(boost::shared_ptr<Windnet::Mysql::DBConnection> conn);

	void flushToCache() {}
	void flushToDB() {}

	int registerAccount(boost::shared_ptr<Windnet::Mysql::DBConnection> conn,
						const std::string &name, const std::string &paswd);

	int createRole(boost::shared_ptr<Windnet::Mysql::DBConnection> conn,
				   AccountInfo::ptr ai, const std::string &roleName, int profession);

	std::string generateUUID();

	AccountInfo::ptr getAccountInfo(const std::string &name);
	AccountInfo::ptr getAccountInfo(int accountId);

	AccountInfo::ptr getAccountInfoByToken(const std::string &token);

	RoleInfo::ptr getRoleInfo(int roleId);
	RoleInfo::ptr getRoleInfo(const std::string &name);

private:
	std::map<std::string, AccountInfo::ptr> m_tokenAccountMap;
	std::map<std::string, RoleInfo::ptr> m_tokenRoleMap;

	std::map<std::string, AccountInfo::ptr> m_nameMap;
	std::map<int, AccountInfo::ptr> m_idMap;

	std::map<int, RoleInfo::ptr> m_roleIdMap;
	std::map<std::string, RoleInfo::ptr> m_roleNameMap;
	std::set<std::string> m_tokens;
};

#endif
