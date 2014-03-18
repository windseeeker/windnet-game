#ifndef __MYSQL_DBCONNECTION_H__
#define __MYSQL_DBCONNECTION_H__
//by william(shc) 2014-2-26

#include <boost/shared_ptr.hpp>

#include <string>
#include <list>

#include <mysql/mysql.h>

namespace Windnet {
namespace Mysql {
class DBConnection {
public:
	typedef boost::shared_ptr<DBConnection> ptr;

	DBConnection();
	~DBConnection() { mysql_close(&m_conn); }

	void connect(const std::string &conninfo, bool sync = true) {}
	void connect(const std::string &host, const std::string &user,
				 const std::string &passwd, const std::string &dbnane);

	void reset() {}
	bool execute(const char *format, ...);

public:
	size_t rows() const;
	size_t columns() const;
	size_t autoId();

	int column(const char *name);

	bool getIsNull(size_t row, size_t column) const;
	bool getIsNull(size_t row, const char *col) {
		return getIsNull(row, column(col));
	}

	template <class T> T getT(size_t row, size_t column) const;
	template <class T> T getT(size_t row, const std::string & col) {
		return getT<T>(row, column(col.c_str()));
	}
	char *get(size_t row, size_t column) const;
	char *get(size_t row, const std::string & col) {
		return get(row, column(col.c_str()));
	}

	short getSmallint(size_t row, size_t column) const;
	short getSmallint(size_t row, const std::string &col) {
		return getSmallint(row, (int)column(col.c_str()));
	}

	int getInteger(size_t row, size_t column) const;
	int getInteger(size_t row, const std::string& col) {
		return getInteger(row, (int)column(col.c_str()));
	}

	float getFloat(size_t row, size_t column) const;
	float getFloat(size_t row, const std::string& col) {
		return getFloat(row, column(col.c_str()));
	}

	std::string getString(size_t row, size_t column) const;
	std::string getString(size_t row, const std::string& col) {
		return getString(row, column(col.c_str()));
	}

private:
	MYSQL m_conn;
	MYSQL_RES *m_result;
};

}

}

#endif
