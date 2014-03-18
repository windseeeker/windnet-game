#ifndef __WINDNET_PQ_CONNECTION_H__
#define __WINDNET_PQ_CONNECTION_H__

#include <string>

struct pg_conn;
struct pg_result;

namespace Windnet {
namespace PQ {
class Connection;

class Result {
friend class Connection;
public:
	Result();

	size_t rows();

	bool isNull(int row, std::string &field);

	template <class T> T get(int row, std::string &field) { return T(1); }
	template <class T> get(int row, int column) { return T(1); }

private:
	pg_result *_result;
};

class Connection {
public:
	bool connect(std::string &dbStr);

	Result execute(const std::string &sql);

private:
	pg_conn *_conn;
};
}

}

#endif
