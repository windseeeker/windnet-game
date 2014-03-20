#ifndef __ITEM_SERVLET_H__
#define __ITEM_SERVLET_H__

#include "servlet.h"

class ServerResource;

class PlayerSession;
class ItemServlet : public Servlet {
public:
	void setupServlet() {}
	bool doRequest(const std::string &type, ServerResource *res, const std::string &token,
				   PlayerSession *ps, Windnet::BSON::Object *request);

protected:
	bool doLoadItems(ServerResource *res, const std::string &token,
					 PlayerSession *ps, Windnet::BSON::Object *request);

	bool doAddItem(ServerResource *res, const std::string &token,
				   PlayerSession *ps, Windnet::BSON::Object *request);

	bool doBuyItem(ServerResource *res, const std::string &token,
				   PlayerSession *ps, Windnet::BSON::Object *request);

	bool doSellItem(ServerResource *res, const std::string &token,
					PlayerSession *ps, Windnet::BSON::Object *request);

	bool doSortItems(ServerResource *res, const std::string &token,
					 PlayerSession *ps, Windnet::BSON::Object *request);

	bool doMoveItem(ServerResource *res, const std::string &token,
					PlayerSession *ps, Windnet::BSON::Object *request);

	bool doStrengthItem(ServerResource *res, const std::string &token,
						PlayerSession *ps, Windnet::BSON::Object *request);

	bool doGetEquipments(ServerResource *res, const std::string &token,
						 PlayerSession *ps, Windnet::BSON::Object *request);

	bool doGetStrengthEquipInfo(ServerResource *res, const std::string &token,
								PlayerSession *ps, Windnet::BSON::Object *request);

	bool doGetEquipGems(ServerResource *res, const std::string &token,
						PlayerSession *ps, Windnet::BSON::Object *request);

	bool doEquipItem(ServerResource *res, const std::string &token,
					 PlayerSession *ps, Windnet::BSON::Object *request);

	bool doUnEquipItem(ServerResource *res, const std::string &token,
					   PlayerSession *ps, Windnet::BSON::Object *request);

	bool doAbandonItem(ServerResource *res, const std::string &token,
					   PlayerSession *ps, Windnet::BSON::Object *request);
};
#endif
