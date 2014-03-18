#include "bson_parser.h"

#include <stdio.h>

#include "../buffer.h"

namespace Windnet {
namespace BSON {
inline bool get(const std::string &key, Object *obj, Value &value) {
	std::map<std::string, Value>::iterator it = obj->find(key);
	if (it != obj->end()) {
		value = it->second;
		return true;
	}
	return false;
}

int getIntVal(const std::string &key, Object *obj) {
	Value val;
	bool ret = get(key, obj, val);
	return ret ? val.get<int>() : 0;
}

bool getBoolVal(const std::string &key, Object *obj) {
	Value val;
	bool ret = get(key, obj, val);
	return ret ? val.get<bool>() : false;
}

double getDoubleVal(const std::string &key, Object *obj) {
	Value val;
	bool ret = get(key, obj, val);
	return ret ? val.get<double>() : 0;
}

std::string getStringVal(const std::string &key, Object *obj) {
	Value val;
	bool ret = get(key, obj, val);
	return ret ? val.get<std::string>() : std::string("");
}

Object getObjectVal(const std::string &key, Object *obj) { ///////////////////todb...check
	Value val;
	bool ret = get(key, obj, val);
	return ret ? val.get<Object>() : Object();
}

Array* getArrayVal(const std::string &key, Object *obj) {
	Value val;
	bool ret = get(key, obj, val);
	return ret ? &val.get<Array>() : NULL;
}

void setIntVal(Object &obj, const std::string &key, int val) {
	Value value;
	value.valueType = BSON_INT;
	value = val;
	obj.insert(std::make_pair(key, value));
}

void setBoolVal(Object &obj, const std::string &key, bool val) {
	Value value;
	value.valueType = BSON_BOOL;
	value = val;
	obj.insert(std::make_pair(key, value));
}

void setDoubleVal(Object &obj, const std::string &key, double val) {
	Value value;
	value.valueType = BSON_DOUBLE;
	value = val;
	obj.insert(std::make_pair(key, value));
}

void setStringVal(Object &obj, const std::string &key, const std::string &val) {
	Value value;
	value.valueType = BSON_STRING;
	value = val;
	obj.insert(std::make_pair(key, value));
}

void setObjectVal(Object &obj, const std::string &key, Object &val) {
	Value value;
	value.valueType = BSON_OBJECT;
	value = val;
	obj.insert(std::make_pair(key, value));
}

void setArrayVal(Object &obj, const std::string &key, Array &val) {
	Value value;
	value.valueType = BSON_ARRAY;
	value = val;
	obj.insert(std::make_pair(key, value));
}

Object parse(const char *buffer, size_t size) {
	bson_iterator iterator;
	bson_iterator *it = &iterator;
	bson_iterator_from_buffer(it, buffer);

	Object result;
	while (bson_iterator_next(it)) {
		const char *key = bson_iterator_key(it);
		Value val = parseValue(it);
		result.insert(std::make_pair(key, val));
	}
	return result;
}

Value parseValue(bson_iterator *it) {
	Value result;
	int type = bson_iterator_type(it);
	switch (type) {
	   case BSON_NULL:
		   break;

	   case BSON_DOUBLE:
		   result = bson_iterator_double(it);
		   break;

	   case BSON_INT:
		   result = bson_iterator_int(it);
		   break;

	   case BSON_LONG:
		   result = bson_iterator_long(it);
		   break;

	   case BSON_BOOL:
		   result = bson_iterator_bool(it);
		   break;

	   case BSON_STRING: {
		   std::string str = bson_iterator_string(it); //necessary
		   result = str;
		   break;
	   }
	   case BSON_DATE:
		   result = bson_iterator_date(it);
		   break;

	   case BSON_BINDATA:
		   printf("current don't supprot binary data\n");
		   break;

	   case BSON_OBJECT: {
		   bson_iterator sub;
		   bson_iterator *subIt = &sub;
		   bson_iterator_subiterator(it, subIt);
		   Object obj;
		   while (bson_iterator_next(subIt)) {
			   Value val = parseValue(subIt);
			   obj.insert(std::make_pair(bson_iterator_key(subIt), val));
		   }
		   result = obj;
		   break;
	   }
	   case BSON_ARRAY: {
		   bson_iterator sub;
		   bson_iterator *subIt = &sub;
		   Array array;
		   while (bson_iterator_next(subIt)) {
			   array.push_back(parseValue(subIt));
		   }
		   result = array;
		   break;
	   }
	}
	return result;
}

void serializeValue(std::string key, Value &value, bson *b);

void serializeToBuffer(Buffer &buffer, Object &obj) {
	bson b;
	bson_init(&b);

	std::map<std::string, Value>::iterator it = obj.begin();
	for (; it != obj.end(); ++it) {
		serializeValue(it->first, it->second, &b);
	}
	bson_finish(&b);
	size_t size = bson_size(&b);
	if (size >= Buffer::MAX_BUFFER_SIZE) {
		printf("Buffer size exceeds MAX BUFFER SIZE, abort\n");
		goto _finish;
	}
	buffer.copyIn(bson_data(&b), size);

_finish:
	bson_destroy(&b);
}

void serializeValue(std::string key, Value &value, bson *b) {
	switch (value.valueType) {
	    case BSON_DOUBLE:
			bson_append_double(b, key.c_str(), value.get<double>());
			break;

		case BSON_INT:
			bson_append_int(b, key.c_str(), value.get<int>());
			break;

	   case BSON_BOOL:
		   bson_append_bool(b, key.c_str(), value.get<bool>());
		   break;

	   case BSON_STRING:
		   bson_append_string(b, key.c_str(), value.get<std::string>().c_str());
		   break;

	    case BSON_NULL:
			bson_append_null(b, key.c_str());
			break;

	    case BSON_DATE:
			bson_append_date(b, key.c_str(), value.get<bson_date_t>());
			break;

	    case BSON_BINDATA:
			break;

	    case BSON_OBJECT: {
			bson_append_start_object(b, key.c_str());
			Object obj = value.get<Object>();
			for (std::map<std::string, Value>::iterator it = obj.begin();
				 it != obj.end(); ++it) {
				serializeValue(it->first, it->second,  b);
			}
			bson_append_finish_object(b);
			break;
		}
	    case BSON_ARRAY: {
			bson_append_start_array(b, key.c_str());
			Array array = value.get<Array>();
			for (size_t i = 0; i < array.size(); ++i) {
				serializeValue("", array[i], b);
			}
			bson_append_finish_array(b);
			break;
		}
	}
}

}
}
