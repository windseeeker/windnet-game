#ifndef __REDFOX_PQ_TRANSACTION_H__
#define __REDFOX_PQ_TRANSACTION_H__
// Copyright (c) 2010 - Uqee, Inc.

namespace Redfox {
namespace PQ {

class Connection;

struct Transaction
{
public:
    enum IsolationLevel
    {
        DEFAULT,
        SERIALIZABLE,
        REPEATABLE_READ = SERIALIZABLE,
        READ_COMMITTED,
        READ_UNCOMMITTED = READ_COMMITTED
    };
    enum Mode
    {
        READ_WRITE,
        READ_ONLY
    };

public:
    Transaction(Connection &connection, IsolationLevel isolationLevel
        = DEFAULT);
    Transaction(Connection &connection, IsolationLevel isolationLevel,
        Mode mode);
    ~Transaction();

    void commit();
    void rollback();

private:
    Connection &m_connection;
    bool m_active;
};

}}

#endif
