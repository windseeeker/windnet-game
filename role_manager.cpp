#include "role_manager.h"

#include <boost/lexical_cast.hpp>

#include <stdio.h>

#include "account_manager.h"
#include "mysql/dbconnection.h"
#include "util.h"

using namespace Windnet;
using namespace Windnet::Mysql;

void RoleManager::loadFromDB(DBConnection::ptr conn) {
	conn->execute("select * from roles");

	size_t rows = conn->rows();
	fprintf(stdout, "Roless %d\n", (int)rows);

	for (size_t i = 0; i < rows; ++i) {
		RoleInfo::ptr roleInfo(new RoleInfo);
		roleInfo->roleId = conn->getInteger(i, "id");
		roleInfo->roleName = conn->getString(i, "name");
		m_idMap[roleInfo->roleId] = roleInfo;
		m_nameMap[roleInfo->roleName] = roleInfo;
	}
}

void RoleManager::flushToDB(DBConnection::ptr conn) {
	std::map<int, RoleInfo::ptr>::iterator it = m_idMap.begin();
	for (; it != m_idMap.end(); ++it) {
		//flushAccountToDB
	}
}

int RoleManager::createRole(DBConnection::ptr conn, const std::string &name,
							const std::string &passwd, const std::string& deviceId) {
	bool ret = conn->execute("insert into accounts(name, passwd, account_state, register_time, token, device_id) "
							 "values('%s', '%s', 1, %d, '%s', '%s');");
	if (!ret) {
		fprintf(stdout, "db executing failed\n");
		return -1;
	}
	RoleInfo::ptr roleInfo(new RoleInfo);
	//accountInfo->accountId = conn->autoId();
	roleInfo->roleName = name;
	//accountInfo->passwd = passwd;
	//m_idMap[accountInfo->accountId] = accountInfo;
	///m_nameMap[accountInfo->accountName] = accountInfo;

	fprintf(stdout, "registerAccount ret %d\n", ret);
	return 0;
}

//Now we only implement single-thread-based login routine, so there is no lock
RoleInfo::ptr RoleManager::getRoleInfo(const std::string &name) {
	std::map<std::string, RoleInfo::ptr>::iterator it = m_nameMap.find(name);
	if (it == m_nameMap.end()) {
		return RoleInfo::ptr();
	}
	return it->second;
}
