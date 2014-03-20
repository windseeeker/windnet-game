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
	bool doRequest(const std::string &type, ServerResource *res,
				   const std::string &token, PlayerSession *ps,  Windnet::BSON::Object *request);

protected:
	bool doRegister(ServerResource *res,  const std::string &token,
					PlayerSession *ps, Windnet::BSON::Object *request);

	bool doValidateName(ServerResource *res, const std::string &token,
						PlayerSession *ps, Windnet::BSON::Object *request);

	bool doLogin(ServerResource *res, const std::string &token,
				 PlayerSession *ps, Windnet::BSON::Object *request);

	bool doAutoLogin(ServerResource *res, const std::string &token,
					 PlayerSession *ps, Windnet::BSON::Object *request);

	bool doLoadServerTowns(ServerResource *res, const std::string &token,
						   PlayerSession *session, Windnet::BSON::Object *request);

	bool doLoadServers(ServerResource *res, const std::string &token,
					   PlayerSession *session, Windnet::BSON::Object *request);

	bool doGetServerInfo(ServerResource *res, const std::string &token,
						 PlayerSession *session, Windnet::BSON::Object *request);

	bool doGetCurrentRecommandSrv(ServerResource *res, const std::string &token,
								  PlayerSession *session, Windnet::BSON::Object *request);

	bool doGetRecommandServers(ServerResource *res, const std::string &token,
							   PlayerSession *session, Windnet::BSON::Object *request);

	bool doChooseServer(ServerResource *res, const std::string &token,
						PlayerSession *session, Windnet::BSON::Object *request);
};

#endif
