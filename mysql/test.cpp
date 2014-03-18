#include <mysql/mysql.h>

#include <boost/lexical_cast.hpp>

#include<stdio.h>

template <class T> T
getValue(MYSQL_RES *_result, size_t row, size_t column) {
    if (!_result) {
        return T(NULL);
    }
	int rows = mysql_num_rows(_result);
	int columns = mysql_num_fields(_result);
    if (row >= rows || column >= columns) {
        return T(NULL);
    }

    MYSQL_ROW r;
    for(int i = 0; i < row; i++) {
        r = mysql_fetch_row(_result);
    }
    return boost::lexical_cast<T>(r[column]);
}

int main() {
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;
	const char *query = "select * from wind_t;";
	int t,r;
	mysql_init(&mysql);

	if (!mysql_real_connect(&mysql,"localhost","root",
							"123456", "wind",0,NULL,0)) {
		printf("Error connecting to database:%s\n",mysql_error(&mysql));
	} else {
		printf("Connected........");
	}

	if (mysql_query(&mysql,query)) {
		printf("Error making query:%s\n", mysql_error(&mysql));
		mysql_close(&mysql);
		return 0;
	}

	printf("Query made ....%d\n", mysql_field_count(&mysql));
	res = mysql_use_result(&mysql);
	if (!res) {
		//char ret = getValue(res, (size_t)1, (size_t)1);
		//printf("%s\n", ret);
		return 0;
	}
	for(int i = 0; i <= mysql_field_count(&mysql); i++) {
		row = mysql_fetch_row(res);
		if(row <=0) {
			//what does this mean?
			break;
		}
		for(int j = 0; j < mysql_num_fields(res); j++) {
			printf("%s ",row[j]);
		}
		printf("\n");
	}

	mysql_free_result(res);
	mysql_close(&mysql);
	return 0;
}
