#ifndef __SOUL_ITEM_SERVLET_H__
#define __SOUL_ITEM_SERVLET_H__

#include "servlet.h"

class ServerResource;

class PlayerSession;
class SoulItemServlet : public Servlet {
public:
	void setupServlet() {}
	bool doRequest(const std::string &type, ServerResource *res, const std::string &token,
				   PlayerSession *ps, Windnet::BSON::Object *request);

protected:
	bool doLoadSoulItems(ServerResource *res, const std::string &token,
						 PlayerSession *ps, Windnet::BSON::Object *request);

	bool doHuntSoulItem(ServerResource *res, const std::string &token,
						PlayerSession *ps, Windnet::BSON::Object *request);

	bool doSwallowSoulItem(ServerResource *res, const std::string &token,
						   PlayerSession *ps, Windnet::BSON::Object *request);

	bool doSwallowAll(ServerResource *res, const std::string &token,
					  PlayerSession *ps, Windnet::BSON::Object *request);

	bool doBuySoulItem(ServerResource *res, const std::string &token,
					   PlayerSession *ps, Windnet::BSON::Object *request);

	bool doEquipSoulItem(ServerResource *res, const std::string &token,
						 PlayerSession *ps, Windnet::BSON::Object *request);

	bool doMoveSoulItem(ServerResource *res, const std::string &token,
                        PlayerSession *ps, Windnet::BSON::Object *request);

	bool doStoreSoulItem(ServerResource *res, const std::string &token,
						 PlayerSession *ps, Windnet::BSON::Object *request);

	bool doGetEquippedSouls(ServerResource *res, const std::string &token,
							PlayerSession *ps, Windnet::BSON::Object *request);
};
#endif
