#include "account_manager.h"

#include <boost/lexical_cast.hpp>

#include <stdio.h>
#include <uuid/uuid.h>

#include "mysql/dbconnection.h"
#include "util.h"

using namespace Windnet;
using namespace Windnet::Mysql;

void AccountManager::loadFromDB(DBConnection::ptr conn) {
	conn->execute("select * from accounts");
	size_t rows = conn->rows();
	fprintf(stdout, "Accounts %d\n", (int)rows);
	for (size_t i = 0; i < rows; ++i) {
		AccountInfo::ptr accountInfo(new AccountInfo);
		accountInfo->accountId = conn->getInteger(i, "id");
		accountInfo->accountName = conn->getString(i, "name");
		accountInfo->passwd = conn->getString(i, "passwd");
		accountInfo->token = conn->getString(i, "token");
		accountInfo->deviceId = conn->getString(i, "device_id");
		accountInfo->sysOpenId = conn->getString(i, "sys_openid");
		m_idMap[accountInfo->accountId] = accountInfo;
		m_nameMap[accountInfo->accountName] = accountInfo;
		m_tokenAccountMap[accountInfo->token] = accountInfo;
		m_tokens.insert(accountInfo->token);
	}
	conn->execute("select * from roles");
	rows = conn->rows();
	fprintf(stdout, "Roles %d\n", (int)rows);
	for (size_t i = 0; i < rows; ++i) {
		int accountId = conn->getInteger(i, "account_id");
		std::map<int, AccountInfo::ptr>::iterator it = m_idMap.find(accountId);
		if (it ==  m_idMap.end()) {
			printf("Role %d has no account\n", accountId);
			continue;
		}
		AccountInfo::ptr ai = it->second;
		RoleInfo::ptr roleInfo(new RoleInfo);
		roleInfo->roleId = conn->getInteger(i, "id");
		roleInfo->level = conn->getInteger(i, "role_lvl");
		roleInfo->profession = conn->getInteger(i, "profession");
		roleInfo->roleName = conn->getString(i, "role_name");
		roleInfo->registerTime = conn->getInteger(i, "last_login_time");
		//roleInfo->lastLogoutTime = conn->getInteger(i, "last_logout_time");
		ai->roles.insert(std::make_pair(roleInfo->roleId, (roleInfo)));
		m_roleIdMap.insert(std::make_pair(roleInfo->roleId, roleInfo));
		m_roleNameMap.insert(std::make_pair(roleInfo->roleName, roleInfo));
	}
}

void AccountManager::flushToDB(DBConnection::ptr conn) {
	std::map<int, AccountInfo::ptr>::iterator it = m_idMap.begin();
	for (; it != m_idMap.end(); ++it) {
		//flushAccountToDB
	}
}

int AccountManager::registerAccount(DBConnection::ptr conn, const std::string &name,
									const std::string &passwd) {
	std::string token = generateUUID();
	std::string sysOpenId = generateUUID();
	bool r;
	//r = conn->execute("insert into accounts(name, passwd, account_state, register_time, token, device_id, open_id,"
	//"device_mode, screen_size, sysopen_id) values('%s', '%s', 1, %d, '%s', '%s', '', '', 0,'%s')",
	//name.c_str(), passwd.c_str(), (int)time(0), token.c_str(), deviceId.c_str(),
	//sysOpenId.c_str());
	r = conn->execute("insert into accounts(name, passwd, account_state, register_time, token, "
					  "sysopen_id) values('%s', '%s', 1, %d, '%s', '%s')", name.c_str(),
					  passwd.c_str(), (int)time(0), token.c_str(), sysOpenId.c_str());
	if (!r) {
		fprintf(stdout, "db executing failed\n");
		return -1;
	}
	AccountInfo::ptr accountInfo(new AccountInfo);
	accountInfo->accountId = conn->autoId();
	accountInfo->accountName = name;
	accountInfo->passwd = passwd;
	accountInfo->token = token;
	accountInfo->sysOpenId = sysOpenId;
	m_idMap[accountInfo->accountId] = accountInfo;
	m_nameMap[accountInfo->accountName] = accountInfo;
	m_tokenAccountMap.insert(std::make_pair(token, accountInfo));
	return 0;
}

int AccountManager::createRole(DBConnection::ptr conn, AccountInfo::ptr ai,
							   const std::string &roleName, int profession) {
	unsigned int now = getCurrentSecond();
	bool ret;
	ret = conn->execute("insert into roles(account_id, role_name, profession, last_login_time) values(%d,"
						"'%s', %d, %d)", ai->accountId, roleName.c_str(), profession, (int)now);
	if (!ret) {
		fprintf(stdout, "create role db executing failed\n");
		return -1;
	}
	RoleInfo::ptr ri(new RoleInfo);
	ri->roleId = conn->autoId();
	ri->roleName = roleName;
	ri->level = 1;
	ri->profession = profession;
	ri->registerTime = now;
	ai->roles.insert(std::make_pair(ri->roleId, ri));
	m_roleIdMap.insert(std::make_pair(ri->roleId, ri));
	m_roleNameMap.insert(std::make_pair(roleName, ri));
	return 0;
}

std::string AccountManager::generateUUID() {
	uuid_t uid;
	uuid_generate(uid);

	char buffer[64] = {0};
	int idx = 0;
    for(int i = 0; i < 16; i++) {
		sprintf(buffer + idx, "%02x", uid[i]);
		idx += 2;
    }
	return std::string(buffer);
	/*
	do {
		//token.clear();
		std::stringstream ss;

		unsigned int current = getCurrentSecond();
		for (size_t i = 0; i < 8; ++i) {
			ss << (char)randInt('a', 'z');
		}
		token.append(ss.str());
		token.append("-");
		token.append(boost::lexical_cast<std::string>(current));
		it = m_tokens.find(token);
	} while (it != m_tokens.end());
*/

}

AccountInfo::ptr AccountManager::getAccountInfoByToken(const std::string &token) {
	std::map<std::string, AccountInfo::ptr>::iterator it = m_tokenAccountMap.find(token);
	if (it == m_tokenAccountMap.end()) {
		return AccountInfo::ptr();
	}
	return it->second;
}

//Now we only implement single-thread-based login routine, so there is no lock
AccountInfo::ptr AccountManager::getAccountInfo(const std::string &name) {
	std::map<std::string, AccountInfo::ptr>::iterator it = m_nameMap.find(name);
	if (it == m_nameMap.end()) {
		return AccountInfo::ptr();
	}
	return it->second;
}

AccountInfo::ptr AccountManager::getAccountInfo(int id) {
	std::map<int, AccountInfo::ptr>::iterator it = m_idMap.find(id);
	if (it == m_idMap.end()) {
		return AccountInfo::ptr();
	}
	return it->second;
}

RoleInfo::ptr AccountManager::getRoleInfo(int id) {
	std::map<int, RoleInfo::ptr>::iterator it = m_roleIdMap.find(id);
	if (it == m_roleIdMap.end()) {
		return RoleInfo::ptr();
	}
	return it->second;

}

RoleInfo::ptr AccountManager::getRoleInfo(const std::string &roleName) {
	std::map<std::string, RoleInfo::ptr>::iterator it = m_roleNameMap.find(roleName);
	if (it == m_roleNameMap.end()) {
		return RoleInfo::ptr();
	}
	return it->second;
}
