#ifndef __SOUL_ITEM_SERVLET_H__
#define __SOUL_ITEM_SERVLET_H__

#include "servlet.h"

class ServerResource;

class PlayerSession;
class SoulItemServlet : public Servlet {
public:
	void setupServlet() {}
	bool doRequest(const std::string &type, boost::shared_ptr<ServerResource> res, const std::string &token,
				   PlayerSession *ps, Windnet::BSON::Object *request);

protected:
	bool doLoadSoulItems(boost::shared_ptr<ServerResource> res, const std::string &token,
						 PlayerSession *ps, Windnet::BSON::Object *request);

	bool doHuntSoulItem(boost::shared_ptr<ServerResource> res, const std::string &token,
						PlayerSession *ps, Windnet::BSON::Object *request);

	bool doSwallowSoulItem(boost::shared_ptr<ServerResource> res, const std::string &token,
						   PlayerSession *ps, Windnet::BSON::Object *request);

	bool doSwallowAll(boost::shared_ptr<ServerResource> res, const std::string &token,
					  PlayerSession *ps, Windnet::BSON::Object *request);

	bool doBuySoulItem(boost::shared_ptr<ServerResource> res, const std::string &token,
					   PlayerSession *ps, Windnet::BSON::Object *request);

	bool doEquiSoulItem(boost::shared_ptr<ServerResource> res, const std::string &token,
						PlayerSession *ps, Windnet::BSON::Object *request);

	bool doMoveSoulItem(boost::shared_ptr<ServerResource> res, const std::string &token,
                        PlayerSession *ps, Windnet::BSON::Object *request);

	bool doStoreSoulItem(boost::shared_ptr<ServerResource> res, const std::string &token,
						 PlayerSession *ps, Windnet::BSON::Object *request);

	bool doGetEquipItemSoul(boost::shared_ptr<ServerResource> res, const std::string &token,
							PlayerSession *ps, Windnet::BSON::Object *request);
};
#endif
