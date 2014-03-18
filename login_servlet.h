#ifndef __LOGIN_SERVLET_H__
#define __LOGIN_SERVLET_H__

#include "servlet.h"

class ServerResource;

namespace Windnet {
	namespace Net {
		class Socket;
	}
}

class PlayerSession;
class LoginServlet : public Servlet {
public:
	void setupServlet();
	bool doRequest(const std::string &type, boost::shared_ptr<ServerResource> res,
				   const std::string &token, PlayerSession *ps,  Windnet::BSON::Object *request);

protected:
	bool doRegister(boost::shared_ptr<ServerResource> res,  const std::string &token,
					PlayerSession *ps, Windnet::BSON::Object *request);

	bool doValidateName(boost::shared_ptr<ServerResource> res, const std::string &token,
						PlayerSession *ps, Windnet::BSON::Object *request);

	bool doLogin(boost::shared_ptr<ServerResource> res, const std::string &token,
				 PlayerSession *ps, Windnet::BSON::Object *request);

	bool doAutoLogin(boost::shared_ptr<ServerResource> res,	 const std::string &token,
					 PlayerSession *ps, Windnet::BSON::Object *request);

	bool doLoadServerTowns(boost::shared_ptr<ServerResource> res, const std::string &token,
						   PlayerSession *session, Windnet::BSON::Object *request);

	bool doLoadServers(boost::shared_ptr<ServerResource> res, const std::string &token,
					   PlayerSession *session, Windnet::BSON::Object *request);

	bool doGetServerInfo(boost::shared_ptr<ServerResource> res, const std::string &token,
						 PlayerSession *session, Windnet::BSON::Object *request);

	bool doGetCurrentRecommandSrv(boost::shared_ptr<ServerResource> res, const std::string &token,
								  PlayerSession *session, Windnet::BSON::Object *request);

	bool doGetRecommandServers(boost::shared_ptr<ServerResource> res, const std::string &token,
							   PlayerSession *session, Windnet::BSON::Object *request);

	bool doChooseServer(boost::shared_ptr<ServerResource> res, const std::string &token,
						PlayerSession *session, Windnet::BSON::Object *request);
};

#endif
