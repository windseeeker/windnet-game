#ifndef __CHAT_SERVLET_H__
#define __CHAT_SERVLET_H__

#include "servlet.h"

class ServerResource;

class PlayerSession;
class ChatServlet : public Servlet {
public:
	void setupServlet() {}
	bool doRequest(const std::string &type, boost::shared_ptr<ServerResource> res, const std::string &token,
				   PlayerSession *ps, Windnet::BSON::Object *request);

protected:
	bool doChat(boost::shared_ptr<ServerResource> res, const std::string &token,
				PlayerSession *ps, Windnet::BSON::Object *request);

};
#endif
