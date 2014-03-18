#include "dbconnection.h"

#include <iostream>

#include <boost/lexical_cast.hpp>

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

namespace Windnet {
namespace Mysql {

DBConnection::DBConnection():m_result(NULL) { }

void DBConnection::connect(const std::string &host, const std::string &user,
						   const std::string &passwd, const std::string &dbname) {
	mysql_init(&m_conn);
	char value = 1;
	mysql_options(&m_conn, MYSQL_OPT_RECONNECT, &value);

	if (!mysql_real_connect(&m_conn, host.c_str(), user.c_str(), passwd.c_str(),
							dbname.c_str(), 0, NULL, 0)) {
		std::cerr << "mysql connect error, " << mysql_error(&m_conn) << std::endl;
		throw;
	}
}

size_t DBConnection::rows() const {
	return m_result ? mysql_num_rows(m_result) : 0;
}

size_t DBConnection::columns() const {
	return m_result ? mysql_num_fields(m_result) : 0;
}

int DBConnection::column(const char* name) {
	if (m_result == NULL || name == NULL) {
		return -1;
	}

	mysql_field_seek(m_result, 0);
	size_t i = 0;
	while (i < columns() ) {
		MYSQL_FIELD *field = mysql_fetch_field(m_result);
		if (field != NULL && strcmp(field->name, name) == 0 ) {
			return i;
		}
		i++;
	}
	return -1;
}

size_t DBConnection::autoId() {
	return mysql_insert_id(&m_conn);
}

bool DBConnection::execute(const char *format, ...) {
	if (m_result != NULL) {
		mysql_free_result(m_result);
		m_result = NULL;
	}
	char buffer[1024];
	va_list arg;
	va_start(arg, format);
	vsprintf(buffer, format, arg);
	va_end(arg);

	if (mysql_query(&m_conn, buffer)) {
		fprintf(stdout, "mysql execute %s error, code %s\n", buffer, mysql_error(&m_conn));
		return false;
	}
	m_result = mysql_store_result(&m_conn);
	if (!m_result && mysql_field_count(&m_conn) != 0) {
		fprintf(stdout, "mysql store resulterror, code %s\n", mysql_error(&m_conn));
		return false;
	}
	return true;
}

template <class T> T
DBConnection::getT(size_t row, size_t column) const {
	if (!m_result) {
		return T(NULL);
	}
	if (row >= rows() || column >= columns()) {
		return T(NULL);
	}
	mysql_data_seek(m_result, row);
	MYSQL_ROW r = mysql_fetch_row(m_result);

	return boost::lexical_cast<T>(r[column]);
}

char *DBConnection::get(size_t row, size_t column) const {
	if (!m_result) {
		return NULL;
	}
	if (row >= rows() || column >= columns()) {
		return NULL;
	}
	mysql_data_seek(m_result, row);
	MYSQL_ROW r = mysql_fetch_row(m_result);
	return r ? r[column] : NULL;
}

bool DBConnection::getIsNull(size_t row, size_t column) const {
	if (!m_result) {
		return true;
	}
	if (row >= rows() || column >= columns()) {
		return true;
	}
	mysql_data_seek(m_result, row);
	MYSQL_ROW r = mysql_fetch_row(m_result);
	return r == NULL;
}

short DBConnection::getSmallint(size_t row, size_t column) const {
	const char *ret = get(row, column);
	if (!ret) {
		printf("get smallint error\n");
		return -1;
	}
	return boost::lexical_cast<short>(get(row, column));
}

int DBConnection::getInteger(size_t row, size_t column) const {
	const char *ret = get(row, column);
	if (ret == NULL) {
		fprintf(stdout, "get integer error\n");
		return -1;
	}
	return boost::lexical_cast<int>(get(row, column));
}

float DBConnection::getFloat(size_t row, size_t column) const {
	const char *ret = get(row, column);
	return ret ? boost::lexical_cast<float>(get(row, column)) : -1;
}

std::string DBConnection::getString(size_t row, size_t column) const {
	const char *ret = get(row, column);
	return ret ? std::string(ret) : std::string("");
}

}
}
