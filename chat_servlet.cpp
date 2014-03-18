#include "chat_servlet.h"

#include <stdio.h>

#include "account_manager.h"
#include "server_resource.h"
#include "socket.h"
#include "player_session.h"

using namespace Windnet;
using namespace Windnet::Net;
bool ChatServlet::doRequest(const std::string &type, ServerResource::ptr res, const std::string &token,
							PlayerSession *ps, BSON::Object *request) {
	if (type == "SendMessage") {
		return doChat(res, token, ps, request);
	}

	fprintf(stdout, "Unknown protocol %s\n", type.c_str());
	fflush(stdout);
	return false;
}

bool ChatServlet::doChat(ServerResource::ptr res, const std::string &token,
						 PlayerSession *ps,  BSON::Object *request) {
	return true;
}
