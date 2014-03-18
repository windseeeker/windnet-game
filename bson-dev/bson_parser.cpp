#include "bson_parser.h"

#include <stdio.h>

namespace Windnet {
namespace BSON {

bool parse(const char *buffer, size_t size, BSONObject& result) {
	bson_iterator iterator;
	bson_iterator *it = &iterator;
	bson_iterator_from_buffer(it, buffer);

	while (bson_iterator_next(it)) {
		const char *key = bson_iterator_key(it);
		Object value = parseValue(it);
		result.insert(std::make_pair(key, value));
	}
	return true;
}

Object parseValue(bson_iterator *it) {
	Object obj;
	switch (bson_iterator_type(it)) {
	   case BSON_NULL:
		   break;

	   case BSON_DOUBLE:
		   obj.value  = bson_iterator_double(it);
		   break;

	   case BSON_INT:
	   case BSON_LONG:
		   obj.value = bson_iterator_int(it);
		   break;

	   case BSON_BOOL:
		   obj.value = bson_iterator_bool(it);
		   break;

	   case BSON_STRING:
		   obj.value = bson_iterator_string(it);
		   break;

	   case BSON_DATE:
		   obj.value = bson_iterator_date(it);
		   break;

	   case BSON_BINDATA:
		   printf("current don't supprot binary data\n");
		   break;

	   case BSON_OBJECT: {
		   bson_iterator sub;
		   bson_iterator *subIt = &sub;
		   bson_iterator_subiterator(it, subIt);
		   Object tmp = parseValue(subIt);
		   boost::get<Object>(value).insert(std::make_pair(bson_iterator_key(it), tmp));
		   break;
	   }
	   case BSON_ARRAY: {
		   bson_iterator sub;
		   bson_iterator *subIt = &sub;
		   while (bson_iterator_next(subIt)) {
			   Object tmp = parseValue(subIt);
			   boost::get<Array>(value).push_back(tmp)
		   }
		   break;
	   }
	}
	return obj;
}
}
}
