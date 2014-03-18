#ifndef __BSON_PARSER_H__
#define __BSON_PARSER_H__
// by william(shc) 2014-3-8

#include <boost/variant.hpp>

#include <map>
#include <vector>

#include "bson/bson.h"

namespace Windnet {

class Buffer;
namespace BSON {

class Value;

typedef std::map<std::string, Value> Object;
typedef std::vector<Value> Array;
typedef boost::variant<bool, int, double, long long, std::string, bson_date_t, Array, Object> ValueBase;

class Value : public ValueBase {
public:
	typedef Object::iterator iterator;

	template <class T>
		Value &operator=(const T &t) {
		(ValueBase &)*this = t;
		return *this;
	}

    template <class T>
		T &get() {
		return boost::get<T>(*this);
	}
	int valueType;
};

int getIntVal(const std::string &key, Object *obj);
bool getBoolVal(const std::string &key, Object *obj);
double getDoubleVal(const std::string &key, Object *obj);
Object getObjectVal(const std::string &key, Object *obj);
Array *getArrayVal(const std::string &key, Object *obj);
std::string getStringVal(const std::string &key, Object *obj);

void setIntVal(Object &obj, const std::string &key, int val);
void setBoolVal(Object &obj, const std::string &key, bool val);
void setDoubleVal(Object &obj, const std::string &key, double val);
void setStringVal(Object &obj, const std::string &key, const std::string &val);
void setObjectVal(Object &obj, const std::string &key, Object &val);
void setArrayVal(Object &obj, const std::string &key, Array &array);

Object parse(const char *buffer, size_t size);
Value parseValue(bson_iterator *it);

void serializeToBuffer(Buffer &buffer, Object &obj);
}
}

#endif
