#ifndef __GEM_SERVLET_H__
#define __GEM_SERVLET_H__

#include "servlet.h"

class ServerResource;

class PlayerSession;
class GemServlet : public Servlet {
public:
	void setupServlet() {}
	bool doRequest(const std::string &type, ServerResource *res, const std::string &token,
				   PlayerSession *ps, Windnet::BSON::Object *request);

protected:
	bool doGetEquipGem(ServerResource *res, const std::string &token,
					   PlayerSession *ps, Windnet::BSON::Object *request);

	bool doEquipGem(ServerResource *res, const std::string &token,
					PlayerSession *ps, Windnet::BSON::Object *request);

	bool doUnEquipGem(ServerResource *res, const std::string &token,
					  PlayerSession *ps, Windnet::BSON::Object *request);

	bool doStrengthGem(ServerResource *res, const std::string &token,
					   PlayerSession *ps, Windnet::BSON::Object *request);
};
#endif
