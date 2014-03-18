#ifndef __WINDNET_PROTOBUF_H__
#define __WINDNET_PROTOBUF_H__

#include "buffer.h"

namespace google {
	namespace Protobufs {
		class Message;
	}
}

namespace Windnet {
class ProtoBuffer : public Buffer {
public:
	void serializeToBuffer() {}
	void parseFromString() { }

private:
	//
};
}

#endif
