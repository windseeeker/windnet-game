#ifndef __ITEM_SERVLET_H__
#define __ITEM_SERVLET_H__

#include "servlet.h"

class ServerResource;

class PlayerSession;
class ItemServlet : public Servlet {
public:
	void setupServlet() {}
	bool doRequest(const std::string &type, boost::shared_ptr<ServerResource> res, const std::string &token,
				   PlayerSession *ps, Windnet::BSON::Object *request);

protected:
	bool doLoadItems(boost::shared_ptr<ServerResource> res, const std::string &token,
					 PlayerSession *ps, Windnet::BSON::Object *request);

	bool doAddItem(boost::shared_ptr<ServerResource> res, const std::string &token,
				   PlayerSession *ps, Windnet::BSON::Object *request);

	bool doBuyItem(boost::shared_ptr<ServerResource> res, const std::string &token,
				   PlayerSession *ps, Windnet::BSON::Object *request);

	bool doSellItem(boost::shared_ptr<ServerResource> res, const std::string &token,
					PlayerSession *ps, Windnet::BSON::Object *request);

	bool doSortItems(boost::shared_ptr<ServerResource> res, const std::string &token,
					 PlayerSession *ps, Windnet::BSON::Object *request);

	bool doMoveItem(boost::shared_ptr<ServerResource> res, const std::string &token,
					PlayerSession *ps, Windnet::BSON::Object *request);

	bool doStrengthItem(boost::shared_ptr<ServerResource> res, const std::string &token,
						PlayerSession *ps, Windnet::BSON::Object *request);

	bool doGetEquipments(boost::shared_ptr<ServerResource> res, const std::string &token,
						 PlayerSession *ps, Windnet::BSON::Object *request);

	bool doGetStrengthEquipInfo(boost::shared_ptr<ServerResource> res, const std::string &token,
								PlayerSession *ps, Windnet::BSON::Object *request);

	bool doGetEquipGems(boost::shared_ptr<ServerResource> res, const std::string &token,
						PlayerSession *ps, Windnet::BSON::Object *request);

	bool doEquipItem(boost::shared_ptr<ServerResource> res, const std::string &token,
					 PlayerSession *ps, Windnet::BSON::Object *request);

	bool doUnEquipItem(boost::shared_ptr<ServerResource> res, const std::string &token,
					   PlayerSession *ps, Windnet::BSON::Object *request);

	bool doAbandonItem(boost::shared_ptr<ServerResource> res, const std::string &token,
					   PlayerSession *ps, Windnet::BSON::Object *request);
};
#endif
