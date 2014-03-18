#include "connection.h"

#include <postgresql/libpq-fe.h>

#include <iostream>

namespace Windnet {
namespace PQ {
Result::Result():_result(NULL) { }

size_t Result::rows() {
	return _result ? PQntuples(_result) : 0;
}

bool Result::isNull(int row, std::string &field) {
	return PQgetisnull(_result, row, PQfnumber(_result, field.c_str());
}

bool Connection::connect(std::string &dbStr) {
	_conn = PQconnectdb(dbStr.c_str());
	assert(_conn);
	if (PQstatus(_conn) != CONNECTION_OK) {\
		std::cout << "Couldn't connect to db, status "
				  << (int)PQstatus(_conn) << ' ' << PQerrorMessage(_conn);
		return false;
	}
	return true;
}

Result Connection::execute(const std::string &sql) {
	Result rt;
	rt._result = PQexec(_conn, sql.c_str());
	return rt;
}

}
}
