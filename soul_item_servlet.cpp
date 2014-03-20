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

bool SoulItemServlet::doRequest(const std::string &type, ServerResource *res, const std::string &token,
								PlayerSession *ps, BSON::Object *request) {
	if (!ps->role()) {
		printf("Playersession has no role\n");
		return false;
	}
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
		return doEquipSoulItem(res, token, ps, request);
	} else if (type == "EquipSoulMetaInfo") {
		return doGetEquippedSouls(res, token, ps, request);
	}
	fprintf(stdout, "Unknown protocol %s\n", type.c_str());
	fflush(stdout);
	return false;
}

bool SoulItemServlet::doLoadSoulItems(ServerResource *res, const std::string &token,
									  PlayerSession *ps,  BSON::Object *request) {
	BSON::Object response, body;
	BSON::Array bagSouls, huntedSouls;
	BSON::setStringVal(response, "Command", "SoulMetaInfo");

	Role *role = ps->role();
	SoulsBag *sb = role->soulsBag();
	std::map<int, SoulItem*> huntedSoulsMap = sb->huntedSouls();
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

bool SoulItemServlet::doHuntSoulItem(ServerResource *res, const std::string &token,
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

		item = new SoulItem(itemTemp);
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

bool SoulItemServlet::doStoreSoulItem(ServerResource *res, const std::string &token,
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

bool SoulItemServlet::doSwallowSoulItem(ServerResource *res, const std::string &token,
										PlayerSession *ps, BSON::Object *request) {
	int swallowId = BSON::getIntVal("Id", request);
	int swallowedId = BSON::getIntVal("BeId", request);

	Role *role = ps->role();
	SoulItem *swallowSoul = role->soulsBag()->getSoul(swallowId);
	if (!swallowSoul) {
		printf("No such swallow from item %d\n", swallowId);
		return true;
	}
	SoulItem *toSoul = role->soulsBag()->getSoul(swallowedId);
	if (!toSoul) {
		printf("No such swallow to item %d\n", swallowedId);
		return true;
	}
	Dataset::SoulItemTemplate *sit = res->getTemplateManager()->getSoulItemTemplate();
	short toLevel = sit->getLevelByExp(toSoul->quality(), toSoul->exp());
	if (toLevel < 0) {
		printf("Can't get level from quality(%d), exp(%d)\n", toSoul->quality(), toSoul->exp());
		return true;
	}
	Dataset::SoulInfo *info = sit->getSoulInfo(toSoul->quality(), toLevel);
	if (!info) {
		printf("Get soul info item error, quality %d, level %d\n", toSoul->quality(), toLevel);
		return true;
	}
	short oldLevel = sit->getLevelByExp(swallowSoul->quality(), swallowSoul->exp());
	swallowSoul->addExp(info->swallowExp + toSoul->exp());

	short newLevel = sit->getLevelByExp(swallowSoul->quality(), swallowSoul->exp());
	if (newLevel != oldLevel) {
		short delta = newLevel - oldLevel;

		Dataset::ItemTemplateManager *itm = res->getTemplateManager()->getItemTemplateManager();
		Dataset::ItemTemplate *it = swallowSoul->getTemplate();
		for (int i = 0; i < delta; ++i) {
			it = itm->get(it->strengthNextLevelId);
			if (!it) {
				printf("No such item id(%d)\n", it->strengthNextLevelId);
				return true;
			}
		}
		swallowSoul->soulId(it->itemId);
		swallowSoul->setTemplate(it);
	}
	swallowSoul->flushToDB(res->getDBConnection(), role->id());

	toSoul->type(SoulItem::SOUL_ITEM_TYPE_DELETED);
	toSoul->flushToDB(res->getDBConnection(), role->id());
	role->soulsBag()->remove(toSoul);

	BSON::Object response, body;
	BSON::setStringVal(response, "Command", "SwallowOne");
	BSON::setIntVal(body, "Id", swallowSoul->id());
	BSON::setIntVal(body, "SoulMetaId", swallowSoul->soulId());
	BSON::setIntVal(body, "Page", swallowSoul->index() / SoulsBag::SOULS_COUNT_PER_PAGE);
	BSON::setIntVal(body, "Index", swallowSoul->index() % SoulsBag::SOULS_COUNT_PER_PAGE);
	BSON::setIntVal(body, "Exp", swallowSoul->exp());
	BSON::setObjectVal(response, "Body", body);

	Buffer buf;
	BSON::serializeToBuffer(buf, response);
	BsonBuffer::serializeToBuffer(buf);
    ps->sendData(buf);
	return true;
}

//TODO optimize
bool SoulItemServlet::doSwallowAll(ServerResource *res, const std::string &token,
								   PlayerSession *ps,  BSON::Object *request) {
	Role *role = ps->role();
	std::map<int, SoulItem*> souls = role->soulsBag()->huntedSouls();
	if (souls.empty()) {
		printf("Empty hunted souls, but swallAll\n");
		return true;
	}
	std::map<int, SoulItem*>::const_iterator it = souls.begin(); 
	SoulItem *swallowSoul = it->second;
	if (!swallowSoul) {
		return true;
	}
	for (++it; it != souls.end(); ++it) {
		if (it->second->quality() > swallowSoul->quality()) {
			swallowSoul = it->second;
		}
	}
	souls.erase(swallowSoul->id());

	Dataset::SoulItemTemplate *sit = res->getTemplateManager()->getSoulItemTemplate();
	short oldLevel = sit->getLevelByExp(swallowSoul->quality(), swallowSoul->exp());

	for (it = souls.begin(); it != souls.end(); ++it) {
		SoulItem *soul = it->second;
		short level = sit->getLevelByExp(soul->quality(), soul->exp());
		if (level < 0) {
			printf("Can't get level from quality(%d), exp(%d)\n", soul->quality(), soul->exp());
			return true;
		}
		Dataset::SoulInfo *info = sit->getSoulInfo(soul->quality(), level);
		if (!info) {
			printf("Get soul info item error, quality %d, level %d\n", soul->quality(), level);
			return true;
		}
		soul->type(SoulItem::SOUL_ITEM_TYPE_DELETED);
		soul->flushToDB(res->getDBConnection(), role->id());
		role->soulsBag()->removeHuntedSoul(soul);

		swallowSoul->addExp(info->swallowExp + soul->exp());
	}
	short newLevel = sit->getLevelByExp(swallowSoul->quality(), swallowSoul->exp());
	if (newLevel != oldLevel) {
        short delta = newLevel - oldLevel;

		Dataset::ItemTemplateManager *itm = res->getTemplateManager()->getItemTemplateManager();
		Dataset::ItemTemplate *it = swallowSoul->getTemplate();
		for (int i = 0; i < delta; ++i) {
			it = itm->get(it->strengthNextLevelId);
			if (!it) {
				printf("No such item id(%d)\n", it->strengthNextLevelId);
				return true;
			}
        }
		swallowSoul->soulId(it->itemId);
		swallowSoul->setTemplate(it);
    }
	role->soulsBag()->removeHuntedSoul(swallowSoul, false);
	role->soulsBag()->storeSoulItem(swallowSoul);
	swallowSoul->flushToDB(res->getDBConnection(), role->id());

	BSON::Object response, body;
	BSON::setStringVal(response, "Command", "SwallowQuickly");
	BSON::setIntVal(body, "Id", swallowSoul->id());
	BSON::setIntVal(body, "SoulMetaId", swallowSoul->soulId());
	BSON::setIntVal(body, "Page", swallowSoul->index() / SoulsBag::SOULS_COUNT_PER_PAGE);
	BSON::setIntVal(body, "Index", swallowSoul->index() % SoulsBag::SOULS_COUNT_PER_PAGE);
	BSON::setIntVal(body, "Exp", swallowSoul->exp());
	BSON::setObjectVal(response, "Body", body);

    Buffer buf;
	BSON::serializeToBuffer(buf, response);
	BsonBuffer::serializeToBuffer(buf);
    ps->sendData(buf);
	return true;
}

bool SoulItemServlet::doBuySoulItem(ServerResource *res, const std::string &token,
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

bool SoulItemServlet::doEquipSoulItem(ServerResource *res, const std::string &token,
									  PlayerSession *ps,  BSON::Object *request) {
	int id = BSON::getIntVal("Id", request);
	int index = BSON::getIntVal("Index", request);

	Role *role = ps->role();
	SoulItem *item = role->soulsBag()->getSoul(id);
	if (!item) {
		printf("No such soul item [%d] to equip\n", id);
		return true;
	}
	if (item->type() == SoulItem::SOUL_ITEM_TYPE_EQUIPPED) {
		printf("Soul item [%d] already equipped\n", id);
		return true;
	}
	role->soulsBag()->remove(item);
	item->index(index);
	item->type(SoulItem::SOUL_ITEM_TYPE_EQUIPPED);
	role->soulsBag()->addEquipSoul(item);
	item->flushToDB(res->getDBConnection(), role->id());

	BSON::Object response, body;
	BSON::setStringVal(response, "Command", "EquipSoulMeta");
	BSON::setBoolVal(body, "Success", true);
	BSON::setObjectVal(response, "Body", body);

	Buffer buf;
	BSON::serializeToBuffer(buf, response);
	BsonBuffer::serializeToBuffer(buf);
    ps->sendData(buf);
	return true;
}

bool SoulItemServlet::doMoveSoulItem(ServerResource *res, const std::string &token,
									 PlayerSession *ps,  BSON::Object *request) {
	int id = BSON::getIntVal("Id", request);
	Role *role = ps->role();
	SoulItem *fromItem = role->soulsBag()->getSoul(id);
	if (!fromItem) {
		printf("No such item [%d] to move\n", id);
		return true;
	}
	short page = BSON::getIntVal("Page", request);
	short idx = BSON::getIntVal("Index", request);
	short toIndex = page * SoulsBag::SOULS_COUNT_PER_PAGE + idx;

	//ASSERT toIndx is a null space
	fromItem->index(toIndex);
	fromItem->flushToDB(res->getDBConnection(), role->id());
	printf("Move soul item [%d], to page(%d) idx(%d)\n", id, page, idx);

	BSON::Object response, body;
	BSON::setStringVal(response, "Command", "SoulMetaMove");
	BSON::setBoolVal(body, "Success", true);
	BSON::setObjectVal(response, "Body", body);

	Buffer buf;
	BSON::serializeToBuffer(buf, response);
	BsonBuffer::serializeToBuffer(buf);
	ps->sendData(buf);
	return true;
}

bool SoulItemServlet::doGetEquippedSouls(ServerResource *res, const std::string &token,
										 PlayerSession *ps,  BSON::Object *request) {
	BSON::Object response, body;
	BSON::Array array;
	BSON::setStringVal(response, "Command", "EquipSoulMetaInfo");

	Role *role = ps->role();
	SoulsBag *sb = role->soulsBag();
	const std::map<int, SoulItem*> soulsMap = sb->equippedSouls();

	for (std::map<int, SoulItem*>::const_iterator it = soulsMap.begin();
         it != soulsMap.end(); ++it) {
		BSON::Object obj;
		BSON::setIntVal(obj, "Id", it->second->id());
		BSON::setIntVal(obj, "SoulMetaId", it->second->soulId());
		BSON::setIntVal(obj, "Index", it->second->index());
		BSON::setIntVal(obj, "Exp", it->second->exp());

		BSON::Value val;
		val = obj;
		val.valueType = BSON_OBJECT;
		array.push_back(val);
    }
	BSON::setArrayVal(body, "SoulMetas", array);
	BSON::setObjectVal(response, "Body", body);
	Buffer buf;
	BSON::serializeToBuffer(buf, response);
	BsonBuffer::serializeToBuffer(buf);
	ps->sendData(buf);
	return true;
}

