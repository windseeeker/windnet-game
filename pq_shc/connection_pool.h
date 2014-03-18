#ifndef __PQ_CONNECTION_POOL_H__
#define __PQ_CONNECTION_POOL_H__

namespace Windnet {
namespace PQ {
class Connection;
class ConnectionPool {
public:
	ConnectionPool(const std::string &connStr, int connNum);

	Connection *getDBConnection();

private:
	std::list<Connection*> _connPool;
};

}
}

#endif
