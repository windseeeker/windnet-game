#ifndef __WINDNET_MESSAGE_H__
#define __WINDNET_MESSAGE_H__

#include "bson/bson_parser.h"

class Session;
class PlayerSession;

namespace Windnet {
namespace Net {
	class Socket;
}

class Message {
public:
	enum {
		STATE_NOT_USED = 0,
		STATE_PENDING = 1
	};

    Message()
		:state(STATE_NOT_USED)
		,ps(NULL) {}

	virtual ~Message() {}

	int state;
	PlayerSession *ps;
	Windnet::BSON::Object request;
};

class SocketMessage {//: public Message {
public:
	//CBsonObject request;
};
}

#endif
