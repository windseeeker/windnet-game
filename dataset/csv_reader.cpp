#include "csv_reader.h"

#include <boost/lexical_cast.hpp>

#include <stdio.h>

bool CSVReader::loadFile(const char *filename) {
	FILE *fp = fopen(filename, "r");
	if (!fp) {
		perror("open");
		return false;
	}
	std::vector<char> content;
	char c;
	do {
		c = fgetc(fp);
		content.push_back(c);
	} while (c != EOF);

	std::string val;
	int idx = 0;
	size_t i = 0;
	char current;
	for (; i < content.size(); ++i) {
		current = content.at(i);
		if (current == 0x0d) {
			if (i + 1 == content.size()) {
				break;
			}
			if (content.at(i + 1) == 0x0a) {
				i += 2;
				//++i; ////////////Bug, i should increase 2
				m_singleline.push_back(val);
				m_fields.insert(std::make_pair(val, idx++));
				break;
			}
		}
		if (current == ',') {
			m_singleline.push_back(val);
			m_fields.insert(std::make_pair(val, idx++));
			val.clear();
			continue;
		}
		val.push_back(content.at(i));
	}
	std::vector<std::string> row;
	val.clear();
	for (; i < content.size(); ++i) {
		current = content.at(i);
		if (current == 0x0d) {
			if (i + 1 == content.size()) {
				break;
            }
			if (content.at(i + 1) == 0x0a) {
				++i;
				row.push_back(val);
				m_rows.push_back(row);
				val.clear();
				row.clear();
				continue;
            }
        }
		if (current == ',') {
			row.push_back(val);
			val.clear();
			continue;
		}
		val.push_back(content.at(i));
	}
	fclose(fp);
	return true;
}

bool CSVReader::Attribute(const char *fieldname, std::string& str) {
	std::map<std::string, int>::iterator it = m_fields.find(fieldname);
	if (it == m_fields.end()) {
		return false;
	}
	str =  m_rows.at(m_currentRow).at(it->second);
	return true;
}

bool CSVReader::Attribute(const char *fieldname, double &val) {
	std::string str;
    if (Attribute(fieldname, str)) {
        val = boost::lexical_cast<double>(str);
        return true;
    }
    val = 0;
    return false;
}

bool CSVReader::Attribute(const char *fieldname, int &val) {
	std::string str;
	if (Attribute(fieldname, str)) {
		if (str.empty()) {
			val = 0;
			return false;
		}
		val = boost::lexical_cast<int>(str);
		return true;
	}
	val = 0;
	return false;
}

bool CSVReader::Attribute(const char *fieldname, short &val) {
	std::string str;
	if (Attribute(fieldname, str)) {
		if (str.empty()) {
			goto _ret;
		}
		val = boost::lexical_cast<short>(str);
		return true;
	}
_ret:
	val = 0;
	return false;
}

bool CSVReader::Attribute(const char *fieldname, char &val) {
	std::string str;
	if (Attribute(fieldname, str)) {
		val = boost::lexical_cast<char>(str);
		return true;
	}
	val = 0;
	return false;
}

bool CSVReader::moveNext() {
	if (++m_currentRow >= (int)m_rows.size()) {
		return false;
	}
	return true;
}
