#include "dbconnection_pool.h"

#include <iostream>

#include "dbconnection.h"

namespace Windnet {
namespace Mysql {
DBConnectionPool::DBConnectionPool(const std::string &host, const std::string &user,
								   const std::string &passwd, const std::string &dbname,
								   size_t dbnum) {
	m_connection.reset(new DBConnection);
	m_connection->connect(host, user, passwd, dbname);
	return;

	//need further processing
	for (size_t i = 0; i < dbnum; ++i) {
		boost::shared_ptr<DBConnection> conn(new DBConnection);
		conn->connect(host, user, passwd, dbname);
		_freeConnection.push_back(conn);
	}
}

boost::shared_ptr<DBConnection> DBConnectionPool::getDBConnection() {
	return m_connection;
	//return boost::shared_ptr<DBConnection>();
	//return 0;
	//return _freeConnection.pop_front();
}

}
}
