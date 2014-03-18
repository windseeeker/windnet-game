#ifndef __FRIEND_SERVLET_H__
#define __FRIEND_SERVLET_H__

#include "servlet.h"

class ServerResource;

class PlayerSession;
class FriendServlet : public Servlet {
public:
	void setupServlet() {}
	bool doRequest(const std::string &type, boost::shared_ptr<ServerResource> res, const std::string &token,
				   PlayerSession *ps, Windnet::BSON::Object *request);

protected:
	bool doAddFriend(boost::shared_ptr<ServerResource> res, const std::string &token,
					 PlayerSession *ps, Windnet::BSON::Object *request);

	bool doAddBlackList(boost::shared_ptr<ServerResource> res, const std::string &token,
						PlayerSession *ps, Windnet::BSON::Object *request);

	bool doRemoveFriend(boost::shared_ptr<ServerResource> res, const std::string &token,
                        PlayerSession *ps, Windnet::BSON::Object *request);

};

#endif
