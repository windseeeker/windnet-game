#include "item_servlet.h"

#include <stdio.h>

#include "account_manager.h"
#include "error_code.h"
#include "item.h"
#include "item_bag.h"
#include "game_template_manager.h"
#include "dataset/item_template_manager.h"
#include "role.h"
#include "server_resource.h"
#include "socket.h"
#include "player_session.h"

using namespace Windnet;
using namespace Windnet::Net;

bool ItemServlet::doRequest(const std::string &type, ServerResource *res, const std::string &token,
							PlayerSession *ps, BSON::Object *request) {
	Role *role = ps->role();
	if (!role) {
		return false;
	}
	if (type == "OpenPackage") {
		return doLoadItems(res, token, ps, request);
	} else if (type == "BuyPropFromBodyShop") {
		return doBuyItem(res, token, ps, request);
	} else if (type == "AppendPackage") {
		return doAddItem(res, token, ps, request);
	} else if (type == "AbandonProps") {
		return doAbandonItem(res, token, ps, request);
	} else if (type == "InBody") {
		return doEquipItem(res, token, ps, request);
	} else if (type == "OutBody") {
		return doUnEquipItem(res, token, ps, request);
	} else if (type == "SortPackage") {
		return doSortItems(res, token, ps, request);
	} else if (type == "MoveProps") {
		return doMoveItem(res, token, ps, request);
	} else if (type == "StrengProps") {
		return doStrengthItem(res, token, ps, request);
	} else if (type == "SaleProps") {
		return doSellItem(res, token, ps, request);
	} else if (type == "GetBody") {
		return doGetEquipments(res, token, ps, request);
	} else if (type == "GetEquipGem") {
		return doGetEquipGems(res, token, ps, request);
	} else if (type == "PrevStrengProps") {
		return doGetStrengthEquipInfo(res, token, ps, request);
	}
	fprintf(stdout, "Unknown protocol %s\n", type.c_str());
	fflush(stdout);
	return false;
}

bool ItemServlet::doLoadItems(ServerResource *res, const std::string &token,
							  PlayerSession *ps,  BSON::Object *request) {
	printf("load items\n");
	BSON::Object response, body;
	BSON::Array array;

	Role *role = ps->role();
	const std::vector<Item::ptr> &items = role->itemBag()->items();
	for (size_t i = 0; i < items.size(); ++i) {
		if (!items[i]) {
			continue;
		}
		BSON::Object obj;
		BSON::setIntVal(obj, "Id", items[i]->id());
		BSON::setIntVal(obj, "PropId", items[i]->itemId());
		BSON::setIntVal(obj, "Number", items[i]->number());
		BSON::setIntVal(obj, "Index", items[i]->index());
		BSON::Value val;
		val.valueType = BSON_OBJECT;
		val = obj;
		array.push_back(val);

		printf("Loadd item globalId %d id %d number %d index %d\n",
			   items[i]->id(), items[i]->itemId(), items[i]->number(), items[i]->index());
	}
	BSON::setStringVal(response, "Command", "OpenPackage");
	BSON::setArrayVal(body, "Packages", array);
	BSON::setObjectVal(response, "Body", body);

	Buffer buf;
	BSON::serializeToBuffer(buf, response);
	BsonBuffer::serializeToBuffer(buf);
	ps->sendData(buf);
	return true;
}

bool ItemServlet::doBuyItem(ServerResource *res, const std::string &token,
                            PlayerSession *ps, BSON::Object *request) {
	int itemId = BSON::getIntVal("Id", request);
	int count = BSON::getIntVal("Count", request);

	Dataset::ItemTemplateManager *itm = res->getTemplateManager()->getItemTemplateManager();
	Dataset::ItemTemplate *it = itm->get(itemId);
	if (!it) {
		printf("No such item %d\n", itemId);
		return true;
	}
	int moneyNeed = it->price * count;
	Role *role = ps->role();
	int roleId = role->roleInfo()->roleId;
	int error = -1;
	BSON::Object response, body;
	BSON::setStringVal(response, "Command", "BuyPropFromBodyShop");
	do {
		if (role->money() < moneyNeed) {
			printf("No enough money to buy item, get %d, but  %d expected\n", role->money(), moneyNeed);
			error = ErrorCode::BUY_ITEM_ERROR_NO_ENOUGH_MONEY;
			break;
		}
		if (role->itemBag()->isFull()) {
			printf("Item bag full\n");
			error = ErrorCode::BUY_ITEM_ERROR_ITEM_BAG_FULL;
			break;
		}
		int val = role->money() - moneyNeed;
		role->money(val);
		role->flushToDB(res->getDBConnection());

		Item::ptr item = Item::createItem(it, roleId, count);
		item->flushNewToDB(res->getDBConnection());
		role->itemBag()->addItem(item);

		BSON::setIntVal(body, "Silver", role->money());
		BSON::setIntVal(body, "Id", item->id());
		BSON::setIntVal(body, "PropId", item->itemId());
		BSON::setIntVal(body, "Index", item->index());
		BSON::setIntVal(body, "Count", item->number());
	} while (0);

	if (error != -1) {
		BSON::setBoolVal(body, "Error", true);
		BSON::setIntVal(body, "ErrorMsg", error);
	}
	BSON::setObjectVal(response, "Body", body);

	Buffer buf;
	BSON::serializeToBuffer(buf, response);
	BsonBuffer::serializeToBuffer(buf);
	ps->sendData(buf);
	fprintf(stdout, "doBuyItem\n");
	return true;
}

bool ItemServlet::doSellItem(ServerResource *res, const std::string &token,
							 PlayerSession *ps, BSON::Object *request) {
	int id = BSON::getIntVal("Id", request);
	//int count = BSON::getIntVal("Number", request);
	fprintf(stdout, "doSellItem id %d\n", id);

	Role *role = ps->role();
	Item::ptr item = role->itemBag()->get(id);
	if (!item) {
		printf("No such item [%d] to sell \n", id);
		return true;
	}
	int price = item->getTemplate()->price * item->number();
	/*
	if (count < 1 || item->number() < count) {
		printf("invalid sell item count %d \n", count);
		return true;
	}
	*/
	role->addMoney(price);
	role->flushToDB(res->getDBConnection());
	item->state(Item::ITEM_STATE_DELETED);
	item->flushToDB(res->getDBConnection());
	role->itemBag()->removeItem(item);

	BSON::Object response, body;
	BSON::setStringVal(response, "Command", "SaleProps");
	BSON::setIntVal(body, "Silver", role->money());
	BSON::setBoolVal(body, "Success", true);
	BSON::setObjectVal(response, "Body", body);

	Buffer buf;
	BSON::serializeToBuffer(buf, response);
	BsonBuffer::serializeToBuffer(buf);
	ps->sendData(buf);
	return true;
}

bool ItemServlet::doAddItem(ServerResource *res, const std::string &token,
							PlayerSession *ps, BSON::Object *request) {
	fprintf(stdout, "doAddItem\n");
	return true;
}

bool ItemServlet::doMoveItem(ServerResource *res, const std::string &token,
							 PlayerSession *ps, BSON::Object *request) {
	int fromId = BSON::getIntVal("Id", request);
	int toIdx = BSON::getIntVal("Index", request);
	fprintf(stdout, "doMoveItem, from %d to %d\n", fromId, toIdx);

	Role *role = ps->role();
	role->itemBag()->moveItem(res->getDBConnection(), fromId, toIdx);

	BSON::Object response, body;
	BSON::setStringVal(response, "Command", "MoveProps");
	BSON::setBoolVal(body, "Success", true);
	BSON::setObjectVal(response, "Body", body);
	Buffer buf;
	BSON::serializeToBuffer(buf, response);
	BsonBuffer::serializeToBuffer(buf);
	ps->sendData(buf);
	return true;
}

bool ItemServlet::doStrengthItem(ServerResource *res, const std::string &token,
								 PlayerSession *ps, BSON::Object *request) {
	int equipId = BSON::getIntVal("Id", request);
	int count = BSON::getIntVal("Count", request);

	fprintf(stdout, "doStrengthItem %d, COUNT %d\n", equipId, count);

	Role *role = ps->role();
	Item::ptr item = role->itemBag()->get(equipId);
	if (!item) {
		printf("No such item %d\n", equipId);
		return true;
	}
	int error = ErrorCode::ERROR_CODE_NO_ERROR;
	Dataset::ItemTemplate *it = item->getTemplate();
	Dataset::ItemTemplateManager *itm = res->getTemplateManager()->getItemTemplateManager();

	int nextId = it->strengthNextLevelId;
	Dataset::ItemTemplate *newItemTemp = NULL;
	short quality = item->quality(), level = item->level();
	int moneyNeed = 0;

	//TODO optimize
	for (int i = 0; i < count; ++i) {
		int equipMoney = itm->getStrengthEquipMoney(quality, level);
		if (equipMoney == -1) {
			printf("can't get equipmoney, strength equip fails\n");
			error = ErrorCode::STRENGTH_EQUIP_ERROR_FAIL;
			goto _result;
		}
		if (level > role->level()) {
			printf("Role level shouldn't execeed item's lvl");
			error = ErrorCode::STRENGTH_EQUIP_ERROR_ROLE_LEVEL_TO_HIGH;
			goto _result;
		}
		moneyNeed += equipMoney;
		if (role->money() < moneyNeed) {
			error = ErrorCode::BUY_ITEM_ERROR_NO_ENOUGH_MONEY;
			goto _result;
		}
		newItemTemp = itm->get(nextId);

        if (!newItemTemp) {
            printf("No such strengthNextLevelId %d\n", nextId);
			return true;
        }
        nextId = newItemTemp->strengthNextLevelId;
	}
	role->money(role->money() - moneyNeed);
	item->quality(quality);
	item->itemId(newItemTemp->itemId);
	item->setTemplate(newItemTemp);
	item->level(level);
	item->flushToDB(res->getDBConnection());

_result:
	BSON::Object response, body;
	BSON::setStringVal(response, "Command", "StrengProps");
	if (error == ErrorCode::ERROR_CODE_NO_ERROR) {
		BSON::setIntVal(body, "Id", equipId);
		BSON::setIntVal(body, "PropId", it->itemId);
		BSON::setIntVal(body, "Quality", item->quality());
		BSON::setIntVal(body, "Level", item->level());
		BSON::setIntVal(body, "Silver", role->money());
	} else {
		BSON::setBoolVal(body, "Error", true);
		BSON::setIntVal(body, "ErrorMsg", error);
	}
	BSON::setObjectVal(response, "Body", body);

	Buffer buf;
	BSON::serializeToBuffer(buf, response);
	BsonBuffer::serializeToBuffer(buf);
    ps->sendData(buf);
	return true;
}

bool ItemServlet::doSortItems(ServerResource *res, const std::string &token,
							  PlayerSession *ps,  BSON::Object *request) {
	Role *role = ps->role();
	ItemBag *itemBag = role->itemBag();
	itemBag->sortItems();

	BSON::Object response, body;
	BSON::Array array;
	const std::vector<Item::ptr> &items = itemBag->items();
	for (size_t i = 0; i < items.size(); ++i) {
		if (!items[i]) {
			continue;
        }
		items[i]->flushToDB(res->getDBConnection()); /////////////////TODO optimizie it
		BSON::Object obj;
		BSON::setIntVal(obj, "Id", items[i]->id());
		BSON::setIntVal(obj, "PropId", items[i]->itemId());
		BSON::setIntVal(obj, "Number", items[i]->number());
		BSON::setIntVal(obj, "Index", items[i]->index());
		BSON::Value val;
		val.valueType = BSON_OBJECT;
		val = obj;
		array.push_back(val);

		printf("Sort item globalId %d id %d number %d index %d\n",
			   items[i]->id(), items[i]->itemId(), items[i]->number(), items[i]->index());
    }
	BSON::setStringVal(response, "Command", "SortPackage");
	BSON::setArrayVal(body, "Packages", array);
	BSON::setObjectVal(response, "Body", body);

    Buffer buf;
	BSON::serializeToBuffer(buf, response);
	BsonBuffer::serializeToBuffer(buf);
    ps->sendData(buf);
	return true;
}

bool ItemServlet::doGetEquipments(ServerResource *res, const std::string &token,
								  PlayerSession *ps,  BSON::Object *request) {
	printf("doGetEquipped Item\n");

	Role *role = ps->role();
	std::vector<Item::ptr> equipments = role->equipments();

	BSON::Object response, body;
	BSON::Array array;
	BSON::setStringVal(response, "Command", "GetBody");
	for (size_t i = 0; i < equipments.size(); ++i) {
		BSON::Object obj;
		BSON::setIntVal(obj, "Id", equipments[i]->id());
		BSON::setIntVal(obj, "Index", equipments[i]->index());
		BSON::Value val;
		val = obj;
		val.valueType = BSON_OBJECT;
		array.push_back(val);
	}
	BSON::setArrayVal(body, "Equips", array);
	BSON::setObjectVal(response, "Body", body);

	Buffer buf;
	BSON::serializeToBuffer(buf, response);
	BsonBuffer::serializeToBuffer(buf);
    ps->sendData(buf);
	return true;
}
bool ItemServlet::doGetStrengthEquipInfo(ServerResource *res, const std::string &token,
										 PlayerSession *ps,  BSON::Object *request) {
	int dbid = BSON::getIntVal("Id", request);

	Item::ptr item = ps->role()->itemBag()->get(dbid);
	if (!item) {
		printf("No such item id %d to strenght info\n", dbid);
		return true;
	}
	BSON::Object response, body;
	BSON::setStringVal(response, "Command", "PrevStrengProps");
	BSON::setIntVal(body, "PrevId", item->itemId());
	BSON::setIntVal(body, "PosId", item->getTemplate()->strengthNextLevelId);
	BSON::setObjectVal(response, "Body", body);

	Buffer buf;
	BSON::serializeToBuffer(buf, response);
	BsonBuffer::serializeToBuffer(buf);
    ps->sendData(buf);
	return true;
}

bool ItemServlet::doEquipItem(ServerResource *res, const std::string &token,
							  PlayerSession *ps,  BSON::Object *request) {
	fprintf(stdout, "doEquipItem\n");
	int dbId = BSON::getIntVal("Id", request);

	Role *role = ps->role();
	Item::ptr item = role->itemBag()->get(dbId);
	if (!item) {
		printf("no such item, dbid %d\n", dbId);
		return true;
	}
	item->isEquip(true);
	item->flushToDB(res->getDBConnection()); ///////////////TODO
	role->itemBag()->removeItem(item, true);
	BSON::Object response, body;
	BSON::setStringVal(response, "Command", "InBody");
	BSON::setBoolVal(body, "Success", true);
	BSON::setIntVal(body, "Id", dbId);
	BSON::setIntVal(body, "PropId", item->itemId());
	BSON::setIntVal(body, "Index", item->index());
	BSON::setObjectVal(response, "Body", body);

	Buffer buf;
	BSON::serializeToBuffer(buf, response);
	BsonBuffer::serializeToBuffer(buf);
	ps->sendData(buf);
	return true;
}

bool ItemServlet::doGetEquipGems(ServerResource *res, const std::string &token,
								 PlayerSession *ps,  BSON::Object *request) {
	BSON::Object response, body;
	BSON::Array array;
	BSON::setStringVal(response, "Command", "GetEquipGem");
	BSON::setArrayVal(response, "Gems", array);
	BSON::setObjectVal(response, "Body", body);

	Buffer buf;
	BSON::serializeToBuffer(buf, response);
	BsonBuffer::serializeToBuffer(buf);
    ps->sendData(buf);
	return false;
}

bool ItemServlet::doUnEquipItem(ServerResource *res, const std::string &token,
								PlayerSession *ps,  BSON::Object *request) {
	int id = BSON::getIntVal("Id", request);
	Role *role = ps->role();
	Item::ptr item = role->itemBag()->get(id);
    if (!item) {
        printf("No such item [%d] to unequip \n", id);
		return true;
	}
	item->isEquip(false);
	item->flushToDB(res->getDBConnection());
	role->itemBag()->addItem(item);

	BSON::Object response, body;
	BSON::setStringVal(response, "Command", "OutBody");
	BSON::setBoolVal(body, "Success", true);
	BSON::setObjectVal(response, "Body", body);

	Buffer buf;
	BSON::serializeToBuffer(buf, response);
	BsonBuffer::serializeToBuffer(buf);
    ps->sendData(buf);
	return false;
}

bool ItemServlet::doAbandonItem(ServerResource *res, const std::string &token,
								PlayerSession *ps,  BSON::Object *request) {
	int id = BSON::getIntVal("Id", request);
    fprintf(stdout, "doAbandonItem id %d\n", id);

	Role *role = ps->role();
	Item::ptr item = role->itemBag()->get(id);
    if (!item) {
		printf("No such item [%d] to abandon \n", id);
		return true;
	}
	item->state(Item::ITEM_STATE_DELETED);
	item->flushToDB(res->getDBConnection());
	role->itemBag()->removeItem(item);

	BSON::Object response, body;
	BSON::setStringVal(response, "Command", "AbandonProps");
	BSON::setBoolVal(body, "Success", true);
	BSON::setObjectVal(response, "Body", body);

    Buffer buf;
	BSON::serializeToBuffer(buf, response);
	BsonBuffer::serializeToBuffer(buf);
    ps->sendData(buf);
	return true;
}
