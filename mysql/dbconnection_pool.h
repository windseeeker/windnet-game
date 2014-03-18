#ifndef __MYSQL_DBCONNECTION_POOL_H__
#define __MYSQL_DBCONNECTION_POOL_H__

#include <boost/shared_ptr.hpp>

#include <string>
#include <list>

#include <mysql/mysql.h>

#include <pthread.h>

namespace Windnet {
namespace Mysql {
class DBConnection;

class DBConnectionPool {
public:
	DBConnectionPool(const std::string &host, const std::string &user,
					 const std::string &passwd, const std::string &dbname, size_t n);
	~DBConnectionPool() {}

	//void start(cnst std::string);

	boost::shared_ptr<DBConnection> getDBConnection();

private:
	boost::shared_ptr<DBConnection> m_connection; //test connection

	std::list<boost::shared_ptr<DBConnection> > _busyConnection;
	std::list<boost::shared_ptr<DBConnection> > _freeConnection;
	std::string _conninfo;
	pthread_mutex_t _mutex;
	pthread_cond_t _cond;
};
}

}

#endif
