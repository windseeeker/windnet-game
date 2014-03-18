#ifndef __BSON_PARSER_H__
#define __BSON_PARSER_H__
// by william(shc) 2014-3-5

#include <boost/variant.hpp>

#include <map>
#include <vector>

#include "bson/bson.h"

namespace Windnet {
namespace BSON {

//lack of viod *

class Object;

//typedef std::map<std::string, Value> Object;
typedef std::vector<Object> Array;

typedef boost::variant<bool, int, double, long long, std::string, bson_date_t, Object, Array> ValueType;

struct Object {
	int type;
	boost::variant<bool, int, double, long long, std::string, bson_date_t, Object, Array> value;
};

typedef std::map<std::string, Object> BSONObject;

bool parse(const char *buffer, size_t size, BSONObject &result);

class Value : public ValueType {
public:
	int getIntValue(const std::string &key);
	bool getBoolValue(const std::string &key);
	double getDoubleValue(const std::string &key);
	std::string getStringValue(const std::string &key);
	void getObjectValue(const std::string &key);

	void setBoolValue(const std::string &key, bool val);
	void setIntValue(const std::string &key, int &val);
	void setDoubleValue(const std::string &key, double &val);
	void setStringValue(const std::string &key, const std::string &value);
	void setObjectValue(const std::string &key);
};

ValueType parse(const char *buffer, size_t size);
Value parseValue(bson_iterator *it);

}
}

#endif
