#include <postgresql/postgres_fe.h>

#include <postgresql/libpq-fe.h>

#include <iostream>

int main_discard() {
	PGconn *conn = PQconnectdb("host=localcast dbname=wind port=5432");
	if (!conn) {
		std::cout << "connect to db failed\n";
		return 0;
	}
	if (PQstatus(conn) != CONNECTION_OK) {
		std::cout << "Invalid connect status " << (int)PQstatus(conn);
		return 0;
	}
	std::cout << "Connected\n";
	return 0;
}

#include <locale.h> 
#include <string> 
#include <sstream>

PGconn *conn; 
PGresult *res; 

void condb() {
	const char *pghost = "localhost"; 
	const char *pgport = "5432"; 
	char *pgoptions = NULL; 
	char *pgtty = NULL; 
	const char *dbName = "wind"; 

	conn = PQsetdbLogin(pghost, pgport, pgoptions, pgtty, dbName,"postgres","postgres"); 
	if (PQstatus(conn) == CONNECTION_BAD) {
		fprintf(stderr, "Connection to database '%s' failed.\n", dbName); 
		fprintf(stderr, "%s", PQerrorMessage(conn)); 
		exit(0);
	} 
} 


/*
void reslist(PGresult *ress) {
	int nFields; 
	res=ress; 
	nFields = PQnfields(res); 
	for (int i = 0; i < nFields; i++) {
		printf("%-15s", PQfname(res, i));
	}
	printf("\n\n"); 

	for (int i = 0; i < PQntuples(res); i++) {
		for (int j = 0; j < nFields; j++) {
			printf("%-15s", PQgetvalue(res, i, j)); 
			text[j]=g_locale_to_utf8(PQgetvalue(res,i,j),-1,NULL,NULL,NULL); 
		} 
		printf("\n"); 
	} 
}

int exit_nicely(PGconn *conn ) {
	PQfinish(conn); 
	exit(1); 
} 

void mysearch(GtkEntry *entry1 ) {
	char *sqltxt, *sqltmp; 
	gchar *txt, *strlocal; 
	char *sqls = malloc(254); 
	sqltxt = "select name,sex,address from test where name like '%"; 
	sqltmp = "%'"; 


	strlocal=g_locale_from_utf8(txt,-1,NULL,NULL,NULL); 
	printf("txt is %s",strlocal); 
	searchtxt = strlocal; 
	strcpy(sqls,sqltxt); 
	strcat(sqls,(char *)searchtxt); 
	strcat(sqls,sqltmp); 
	printf("sqltxt is %s\n",sqls); 
	conres(conn,sqls); 
	reslist(res); 
	PQclear(res); 
	free(sqls); 
} 

void destroy( GtkWidget *widget, 
			  gpointer data ) 
{ 
	PQfinish(conn); 
} 
*/
int main( int argc, 
		  char *argv[] ) 
{ 
	condb(); 

	std::stringstream ss;
	ss << "select * from test;";

	res = PQexec(conn, ss.str().c_str());
	if (!res || PQresultStatus(res) != PGRES_TUPLES_OK) {
		std::cerr << "BEGIN command failed " << PQresultStatus(res) << std::endl;
        PQclear(res);
    }

	return 0; 
}
