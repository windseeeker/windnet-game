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
	bool doRequest(const std::string &type, boost::shared_ptr<ServerResource> res, const std::string &token,
				   PlayerSession *ps, Windnet::BSON::Object *request);

protected:
	bool doCreateRole(boost::shared_ptr<ServerResource> res, const std::string &token,
					  PlayerSession *ps, Windnet::BSON::Object *request);

	bool doValidateNickName(boost::shared_ptr<ServerResource> res, const std::string &token,
							PlayerSession *ps, Windnet::BSON::Object *request);

	bool doLoadRoles(boost::shared_ptr<ServerResource> res, const std::string &token,
					 PlayerSession *ps, Windnet::BSON::Object *request);

	bool doChooseRole(boost::shared_ptr<ServerResource> res, const std::string &token,
					  PlayerSession *ps, Windnet::BSON::Object *request);

	bool doRemoveRole(boost::shared_ptr<ServerResource> res, const std::string &token,
					  PlayerSession *ps, Windnet::BSON::Object *request);

	bool doRecoveryRole(boost::shared_ptr<ServerResource> res, const std::string &token,
						PlayerSession *ps, Windnet::BSON::Object *request);
};

#endif
