#include "soul_item_servlet.h"

#include <stdio.h>

#include "account_manager.h"
#include "dataset/item_template_manager.h"
#include "dataset/soul_item_template.h"
#include "error_code.h"
#include "game_template_manager.h"
#include "role.h"
#include "server_resource.h"
#include "souls_bag.h"
#include "player_session.h"
#include "util.h"

using namespace Windnet;
using namespace Windnet::Net;

bool SoulItemServlet::doRequest(const std::string &type, ServerResource::ptr res, const std::string &token,
								PlayerSession *ps, BSON::Object *request) {
	if (type == "SoulMetaInfo") {
		return doLoadSoulItems(res, token, ps, request);
	} else if (type == "LotterySoulMeta") {
		return doHuntSoulItem(res, token, ps, request);
	} else if (type == "PacketSoulMeta") {
		return doStoreSoulItem(res, token, ps, request);
	} else if (type == "SwallowQuickly") {
		return doSwallowAll(res, token, ps, request);
	} else if (type == "SwallowOne") {
		return doSwallowSoulItem(res, token, ps, request);
	} else if (type == "SoulMetaMove") {
		return doMoveSoulItem(res, token, ps, request);
	} else if (type == "BuySoulMeta") {
		return doBuySoulItem(res, token, ps, request);
	} else if (type == "EquipSoulMeta") {
		return doEquiSoulItem(res, token, ps, request);
	} else if (type == "EquipSoulMetaInfo") {
		return doGetEquipItemSoul(res, token, ps, request);
	}

	fprintf(stdout, "Unknown protocol %s\n", type.c_str());
	fflush(stdout);
	return false;
}

bool SoulItemServlet::doLoadSoulItems(ServerResource::ptr res, const std::string &token,
									  PlayerSession *ps,  BSON::Object *request) {
	BSON::Object response, body;
	BSON::Array bagSouls, huntedSouls;
	BSON::setStringVal(response, "Command", "SoulMetaInfo");

	Role *role = ps->role();
	SoulsBag *sb = role->soulsBag();
	const std::map<int, SoulItem*> huntedSoulsMap = sb->huntedSouls();
	const std::map<int, SoulItem*> soulsBag = sb->soulsBag();

	for (std::map<int, SoulItem*>::const_iterator it = huntedSoulsMap.begin();
		 it != huntedSoulsMap.end(); ++it) {
		BSON::Object obj;
		BSON::setIntVal(obj, "Id", it->second->id());
		BSON::setIntVal(obj, "SoulMetaId", it->second->soulId());
		BSON::setIntVal(obj, "Index", it->second->index());
		BSON::setIntVal(obj, "Exp", it->second->exp());

		BSON::Value val;
		val = obj;
		val.valueType = BSON_OBJECT;
		huntedSouls.push_back(val);
	}
	for (std::map<int, SoulItem*>::const_iterator it = soulsBag.begin();
         it != soulsBag.end(); ++it) {
		BSON::Object obj;
		BSON::setIntVal(obj, "Id", it->second->id());
		BSON::setIntVal(obj, "SoulMetaId", it->second->soulId());
		BSON::setIntVal(obj, "Exp", it->second->exp());
		//TODO.......
		BSON::setIntVal(obj, "Page", it->second->index() / SoulsBag::SOULS_COUNT_PER_PAGE);
		BSON::setIntVal(obj, "Index", it->second->index() % SoulsBag::SOULS_COUNT_PER_PAGE);

		BSON::Value val;
		val = obj;
		val.valueType = BSON_OBJECT;
		bagSouls.push_back(val);
	}
	BSON::setIntVal(body, "SoulMetaOpen", role->soulsIndex());
	BSON::setIntVal(body, "SoulPoint", role->soulsValue());
	BSON::setArrayVal(body, "SoulMetas", huntedSouls);
	BSON::setArrayVal(body, "SoulMetaPacks", bagSouls);
	BSON::setObjectVal(response, "Body", body);

	Buffer buf;
	BSON::serializeToBuffer(buf, response);
	BsonBuffer::serializeToBuffer(buf);
	ps->sendData(buf);
	return true;
}

bool SoulItemServlet::doHuntSoulItem(ServerResource::ptr res, const std::string &token,
									 PlayerSession *ps,  BSON::Object *request) {
	int id = BSON::getIntVal("SoulMetaIndex", request);

	printf("hunt soul item idx %d\n", id);
	Dataset::ItemTemplateManager *itm = res->getTemplateManager()->getItemTemplateManager();
	Dataset::SoulItemTemplate *sit = res->getTemplateManager()->getSoulItemTemplate();
	Dataset::SoulHuntInfo * huntInfo = sit->getSoulHuntInfo(id);
	if (!huntInfo) {
		printf("No such soulItem id [%d] to obtain\n", id);
		return true;
	}
	Dataset::SoulHuntInfo *nextHuntInfo = sit->getSoulHuntInfo(id + 1);
	if (nextHuntInfo) {
		huntInfo = nextHuntInfo;
	}
	Role *role = ps->role();
	SoulsBag *sb = role->soulsBag();
	int error = ErrorCode::ERROR_CODE_NO_ERROR;
	SoulItem *item = 0;
	do {
		if (role->money() < huntInfo->moneyNeed) {
			error = ErrorCode::ERROR_NO_ENOUGH_MONEY;
			break;
		}
		if (sb->isFull()) {
			error = ErrorCode::BUY_ITEM_ERROR_ITEM_BAG_FULL;
			break;
		}
		int weight = randInt(0, huntInfo->randQuality.rbegin()->first);
		std::map<short, short>::iterator it = huntInfo->randQuality.upper_bound(weight);
		if (it == huntInfo->randQuality.end()) {
			printf("Fatal error when get random quality\n");
			return true;
		}
		Dataset::ItemTemplate *itemTemp = itm->getRandomSoulItemByQuality(it->second);
		if (!itemTemp) {
			printf("Fatal error when get random soul item from quality\n");
			return true;
		}
		if (role->level() >= huntInfo->levelNeed && randDouble() < huntInfo->levelUpRate) {
			role->addSoulsIndex();
		}
		role->addMoney(-huntInfo->moneyNeed);
		role->addSoulsValue(huntInfo->soulValue);
		role->flushToDB(res->getDBConnection());

		item = new SoulItem;
		item->soulId(itemTemp->itemId);
		item->index(sb->nextHuntSoulIndex());
		item->flushNewToDB(res->getDBConnection(), role->id());
		sb->addSoulItem(item);
	} while (0);
	BSON::Object response, body;
	BSON::setStringVal(response, "Command", "LotterySoulMeta");

	if (error == ErrorCode::ERROR_CODE_NO_ERROR) {
		BSON::setIntVal(body, "Silver", role->money());
		BSON::setIntVal(body, "SoulMetaOpen", role->soulsIndex());
		BSON::setIntVal(body, "SoulPoint", role->soulsValue());
		BSON::setIntVal(body, "SoulMetaId", item->soulId());
		BSON::setIntVal(body, "Id", item->id());
		BSON::setIntVal(body, "Index", item->index());
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

bool SoulItemServlet::doStoreSoulItem(ServerResource::ptr res, const std::string &token,
									  PlayerSession *ps,  BSON::Object *request) {
	int id = BSON::getIntVal("Id", request);

	Role *role = ps->role();
	SoulItem *item = role->soulsBag()->getHuntSoul(id);
	if (!item) {
		printf("No such soul item %d\n", id);
		return true;
	}
	BSON::Object response, body;
	BSON::setStringVal(response, "Command", "PacketSoulMeta");

	bool ret = role->soulsBag()->storeSoulItem(item);
	if (ret) {
		item->flushToDB(res->getDBConnection(), role->id());
		BSON::setIntVal(body, "SoulMetaId", item->soulId());
		BSON::setIntVal(body, "Id", item->id());
		BSON::setIntVal(body, "Exp", item->exp());
		BSON::setIntVal(body, "Page", item->index() / SoulsBag::SOULS_COUNT_PER_PAGE);
		BSON::setIntVal(body, "Index", item->index() % SoulsBag::SOULS_COUNT_PER_PAGE);
	} else {
		BSON::setBoolVal(body, "Error", true);
		BSON::setIntVal(body, "ErrorMsg", ErrorCode::BUY_ITEM_ERROR_ITEM_BAG_FULL);
	}
	BSON::setObjectVal(response, "Body", body);

	Buffer buf;
	BSON::serializeToBuffer(buf, response);
	BsonBuffer::serializeToBuffer(buf);
    ps->sendData(buf);
	return true;
}

bool SoulItemServlet::doSwallowSoulItem(ServerResource::ptr res, const std::string &token,
										PlayerSession *ps, BSON::Object *request) {
	BSON::Object response;
	Buffer buf;
	BSON::serializeToBuffer(buf, response);
	BsonBuffer::serializeToBuffer(buf);
    ps->sendData(buf);
	return true;
}

bool SoulItemServlet::doSwallowAll(ServerResource::ptr res, const std::string &token,
								   PlayerSession *ps,  BSON::Object *request) {
	return true;
}

bool SoulItemServlet::doBuySoulItem(ServerResource::ptr res, const std::string &token,
									PlayerSession *ps,  BSON::Object *request) {
	int soulItemId = BSON::getIntVal("SoulMetaId", request);
	int buyCount = BSON::getIntVal("Count", request);

	Dataset::ItemTemplateManager *itm = res->getTemplateManager()->getItemTemplateManager();
	Dataset::ItemTemplate *it = itm->get(soulItemId);
	if (!it) {
		printf("no such soul item id %d\n", soulItemId);
		return true;
	}
	Role *role = ps->role();
	int moneyNeed = it->price * buyCount;
	if (role->money() > moneyNeed) {
		return true;
	}

	BSON::Object response, body;
	BSON::setStringVal(response, "Command", "BuySoulMeta");
	BSON::setIntVal(body, "SoulPoint", 1);
	Buffer buf;
	BSON::serializeToBuffer(buf, response);
	BsonBuffer::serializeToBuffer(buf);
    ps->sendData(buf);

	//BSON::setIntVal(
	return true;
}

bool SoulItemServlet::doEquiSoulItem(ServerResource::ptr res, const std::string &token,
									 PlayerSession *ps,  BSON::Object *request) {
	//int soulId = BSON::getIntVal("EquipSoulMeta", request);
	//int index = BSON::getIntVal("Index", request);



	return true;
}

bool SoulItemServlet::doMoveSoulItem(ServerResource::ptr res, const std::string &token,
									 PlayerSession *ps,  BSON::Object *request) {
	//int soulId = BSON::getIntVal("Id", request);
	//int page = BSON::getIntVal("Page", request);
	//int index = BSON::getIntVal("Index", request);

	return true;
}

bool SoulItemServlet::doGetEquipItemSoul(ServerResource::ptr res, const std::string &token,
										 PlayerSession *ps,  BSON::Object *request) {
	BSON::Object response, body;
	BSON::Array array;
	BSON::setStringVal(response, "Command", "EquipSoulMetaInfo");

	BSON::setArrayVal(body, "SoulMetas", array);
	BSON::setObjectVal(response, "Body", body);
	Buffer buf;
	BSON::serializeToBuffer(buf, response);
	BsonBuffer::serializeToBuffer(buf);
	ps->sendData(buf);
	return true;
}

