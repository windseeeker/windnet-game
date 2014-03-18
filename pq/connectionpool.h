#ifndef __REDFOX_PQ_CONNECTIONPOOL_H__
#define __REDFOX_PQ_CONNECTIONPOOL_H__

#include <list>

#include <boost/shared_ptr.hpp>

#include "redfox/fibersynchronization.h"

namespace Redfox {

class IOManager;

namespace PQ {

class Connection;

class ConnectionPool : boost::noncopyable {
public:
    typedef boost::shared_ptr<ConnectionPool> ptr;

    ConnectionPool(const std::string &conninfo, IOManager *iomanager,
        size_t size = 5);
    ~ConnectionPool();
    boost::shared_ptr<Connection> getConnection();
    void resize(size_t num);

private:
    void releaseConnection(Redfox::PQ::Connection* conn);

private:
    std::list<boost::shared_ptr<Redfox::PQ::Connection> > m_busyConnections;
    std::list<boost::shared_ptr<Redfox::PQ::Connection> > m_freeConnections;
    std::string m_conninfo;
    IOManager *m_iomanager;
    FiberMutex m_mutex;
    FiberCondition m_condition;
    size_t m_total;
};

}}

#endif
