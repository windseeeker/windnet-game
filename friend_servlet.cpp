#include "friend_servlet.h"

#include <stdio.h>

#include "account_manager.h"
#include "server_resource.h"
#include "socket.h"
#include "player_session.h"

using namespace Windnet;
using namespace Windnet::Net;

bool FriendServlet::doRequest(const std::string &type, ServerResource::ptr res, const std::string &token,
							  PlayerSession *ps, BSON::Object *request) {
	if (type == "AddFriend") {
		return doAddFriend(res, token, ps, request);
	} else if (type == "AddStrangers") {
		return doAddBlackList(res, token, ps, request);
	} else if (type == "RemoveFriend") {
		return doRemoveFriend(res, token, ps, request);
	}

	fprintf(stdout, "Unknown protocol %s\n", type.c_str());
	fflush(stdout);
	return false;
}

bool FriendServlet::doAddFriend(ServerResource::ptr res, const std::string &token,
								PlayerSession *ps,  BSON::Object *request) {
	return true;
}

bool FriendServlet::doAddBlackList(ServerResource::ptr res, const std::string &token,
								   PlayerSession *ps,  BSON::Object *request) {
	return true;
}

bool FriendServlet::doRemoveFriend(ServerResource::ptr res, const std::string &token,
								   PlayerSession *ps,  BSON::Object *request) {
	return true;
}

