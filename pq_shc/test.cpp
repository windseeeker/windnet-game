#include <postgresql/libpq-fe.h>

#include <iostream>

int main() {
	PGconn *conn = PQconnectdb("dbname=wind user=postgres");
	if (!conn) {
		std::cout << "connect to db failed\n";
		return 0;
	}
	if (PQstatus(conn) != CONNECTION_OK) {
		std::cout << "Invalid connect status " << (int)PQstatus(conn) << std::endl
				  << PQerrorMessage(conn);
		return 0;
	}
	std::cout << "Connected\n";


	PGresult *result = PQexec(conn, "select * from test");
	if (!result || PQresultStatus(result) != PGRES_TUPLES_OK) {
		std::cout << "exe sql failed " << PQerrorMessage(conn) << std::endl;
		return 0;
	}

	//Fields
	int fields = PQnfields(result);
	std::cout << "table has fields of " << fields << std::endl;

	//result records, (rows)
	int rows = PQntuples(result);
	std::cout << "table has rows of  " << rows << std::endl;

	for (int i = 0; i < rows; ++i) {

		int n = PQfnumber(result, "id");
		printf("%-15s", PQgetvalue(result, i, n));

		n = PQfnumber(result, "idx");
		printf("%-15s", PQgetvalue(result, i, n));
	}
	printf("\n");

	return 0;
}
