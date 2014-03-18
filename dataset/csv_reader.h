#ifndef __CSV_READER_H__
#define __CSV_READER_H__
//by william(shc) 2014-3-3

#include <map>
#include <string>
#include <vector>

class CSVReader {
public:
    CSVReader():m_currentRow(0) {}
	~CSVReader() {}

	bool loadFile(const char *filename);

	bool Attribute(const char *fieldname, std::string &str);
	bool Attribute(const char *fieldname, int &val);
	bool Attribute(const char *fieldname, short &val);
	bool Attribute(const char *fieldname, char &val);
	bool Attribute(const char *fieldname, double &val);

	bool moveNext();

private:
	int m_currentRow;
	std::map<std::string, int> m_fields;
	std::vector<std::string> m_singleline;
	std::vector<std::vector<std::string> > m_rows;
};

#endif
