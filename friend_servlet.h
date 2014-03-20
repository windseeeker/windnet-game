#ifndef __FRIEND_SERVLET_H__
#define __FRIEND_SERVLET_H__

#include "servlet.h"

class ServerResource;

class PlayerSession;
class FriendServlet : public Servlet {
public:
	void setupServlet() {}
	bool doRequest(const std::string &type, ServerResource *res, const std::string &token,
				   PlayerSession *ps, Windnet::BSON::Object *request);

protected:
	bool doAddFriend(ServerResource *res, const std::string &token,
					 PlayerSession *ps, Windnet::BSON::Object *request);

	bool doAddBlackList(ServerResource *res, const std::string &token,
						PlayerSession *ps, Windnet::BSON::Object *request);

	bool doRemoveFriend(ServerResource *res, const std::string &token,
                        PlayerSession *ps, Windnet::BSON::Object *request);

};

#endif
