#ifndef __ROLE_SERVLET_H__
#define __ROLE_SERVLET_H__

#include "servlet.h"

class ServerResource;

namespace Windnet {
	namespace Net {
		class Socket;
	}
}

class PlayerSession;
class RoleServlet : public Servlet {
public:
	enum {
		error,
	};

	void setupServlet();
	bool doRequest(const std::string &type, ServerResource *res, const std::string &token,
				   PlayerSession *ps, Windnet::BSON::Object *request);

protected:
	bool doCreateRole(ServerResource *res, const std::string &token,
					  PlayerSession *ps, Windnet::BSON::Object *request);

	bool doValidateNickName(ServerResource *res, const std::string &token,
							PlayerSession *ps, Windnet::BSON::Object *request);

	bool doLoadRoles(ServerResource *res, const std::string &token,
					 PlayerSession *ps, Windnet::BSON::Object *request);

	bool doChooseRole(ServerResource *res, const std::string &token,
					  PlayerSession *ps, Windnet::BSON::Object *request);

	bool doRemoveRole(ServerResource *res, const std::string &token,
					  PlayerSession *ps, Windnet::BSON::Object *request);

	bool doRecoveryRole(ServerResource *res, const std::string &token,
						PlayerSession *ps, Windnet::BSON::Object *request);
};

#endif
