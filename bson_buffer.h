#ifndef __WINDNET_BSON_BUFFER_H__
#define __WINDNET_BSON_BUFFER_H__

#include "buffer.h"

#include <map>

//#include "bson/CBsonObject.h"
#include "bson/bson_parser.h"

class CBsonObject;

namespace Windnet {
class BsonBuffer : public Buffer {
public:
	BSON::Object &protocol() { return m_protocol; }

	bool parseable() { return readAvailable() >= 10; }

	int parseFromBuffer(int id);

	static void serializeToBuffer(CBsonObject &obj, Buffer& buf);
	static void serializeToBuffer(Buffer& buf);

private:
	BSON::Object m_protocol;
};
}

#endif
