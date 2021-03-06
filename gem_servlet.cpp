#include "gem_servlet.h"

#include <stdio.h>

#include "dataset/item_template_manager.h"
#include "error_code.h"
#include "game_template_manager.h"
#include "item.h"
#include "item_bag.h"
#include "role.h"
#include "server_resource.h"
#include "player_session.h"

using namespace Windnet;
using namespace Windnet::Net;

Dataset::ItemTemplate *g_fragmentTemplate = NULL;

bool GemServlet::doRequest(const std::string &type, ServerResource *res, const std::string &token,
						   PlayerSession *ps, BSON::Object *request) {
	if (type == "GetEquipGem") {
		return doGetEquipGem(res, token, ps, request);
	} else if (type == "UpGem") {
		return doEquipGem(res, token, ps, request);
	} else if (type == "DownGem") {
		return doUnEquipGem(res, token, ps, request);
	} else if (type == "StrengGem") {
		return doStrengthGem(res, token, ps, request);
	}

	fprintf(stdout, "Unknown protocol %s\n", type.c_str());
	fflush(stdout);
	return false;
}

bool GemServlet::doGetEquipGem(ServerResource *res, const std::string &token,
							   PlayerSession *ps,  BSON::Object *request) {
	int id = BSON::getIntVal("Id", request);
	Role *role = ps->role();
	Item::ptr item = role->itemBag()->get(id);
	if (!item) {
		printf("No such equip %d\n", id);
		return true;
	}
	const std::vector<Gem::ptr> gems = item->gems();

	BSON::Object response, body;
	BSON::Array array;
	BSON::setStringVal(response, "Command", "GetEquipGem");

	for (size_t i = 0; i < gems.size(); ++i) {
		BSON::Object obj;
		BSON::setIntVal(obj, "Id", gems[i]->id);
		BSON::setIntVal(obj, "PropId", gems[i]->gemId);
		BSON::setIntVal(obj, "Index", gems[i]->pos);
		BSON::setIntVal(obj, "Exp", gems[i]->exp);

		BSON::Value val;
		val = obj;
		val.valueType = BSON_OBJECT;
		array.push_back(val);
	}
	BSON::setArrayVal(body, "Gems", array);
	BSON::setObjectVal(response, "Body", body);

	Buffer buf;
	BSON::serializeToBuffer(buf, response);
	BsonBuffer::serializeToBuffer(buf);
	ps->sendData(buf);
	return true;
}

bool GemServlet::doEquipGem(ServerResource *res, const std::string &token,
							PlayerSession *ps,  BSON::Object *request) {
	printf("do Equip gem\n");

	int equipId = BSON::getIntVal("EquipId", request);
	int gemId = BSON::getIntVal("GemId", request);
	int idx = BSON::getIntVal("Index", request);

	Role *role = ps->role();
	Item::ptr equipItem = role->itemBag()->get(equipId);
	if (!equipItem) {
		printf("Role has no gem [%d] to equip gem\n", equipId);
		return true;
	}
	Item::ptr gemItem = role->itemBag()->get(gemId);
	if (!gemItem) {
		printf("Role has no gem [%d] to equip gem\n", gemId);
		return true;
	}
	Gem::ptr gem(new Gem);
	gem->id = gemItem->id(); ////////////////////globalId is gem item's id
	gem->gemId = gemId;
	gem->pos = idx;
	gem->exp = 0;
	if (!equipItem->addGem(gem)) {
		printf("equip gem error idx \n");
		return true;
	}
	role->itemBag()->removeItem(gemItem);
	gemItem->state(Item::ITEM_STATE_DELETED);
	gemItem->flushToDB(res->getDBConnection());
	equipItem->flushToDB(res->getDBConnection());

	BSON::Object response, body;
	BSON::setStringVal(response, "Command", "UpGem");
	BSON::setIntVal(body, "EquipId", equipId);
	BSON::setIntVal(body, "Index", idx);
	BSON::setIntVal(body, "Exp", 0);
	BSON::setObjectVal(response, "Body", body);

	Buffer buf;
	BSON::serializeToBuffer(buf, response);
	BsonBuffer::serializeToBuffer(buf);
    ps->sendData(buf);
	return true;
}

bool GemServlet::doUnEquipGem(ServerResource *res, const std::string &token,
							  PlayerSession *ps, BSON::Object *request) {
	printf("doUnEquip gem\n");

	int equipId = BSON::getIntVal("EquipId", request);
	int gemId = BSON::getIntVal("GemId", request);

	Role *role = ps->role();
	Item::ptr equipItem = role->itemBag()->get(equipId);
	if (!equipItem) {
		printf("Role has no gem [%d] to equip gem\n", equipId);
		return true;
	}
	Item::ptr gemItem = role->itemBag()->get(gemId);
	if (!gemItem) {
		printf("Role has no gem [%d] to equip gem\n", gemId);
		return true;
	}
	return true;
}

bool GemServlet::doStrengthGem(ServerResource *res, const std::string &token,
							   PlayerSession *ps,  BSON::Object *request) {
	printf("doStrength Gem\n");

	int equipId = BSON::getIntVal("EquipId", request);
	int gemId = BSON::getIntVal("GemId", request);
	int count = BSON::getIntVal("Count", request);

	Role *role = ps->role();
	Item::ptr item = role->itemBag()->get(equipId);
	if (!item) {
		printf("Role has no gem [%d] to strength\n", equipId);
		return true;
	}
	Gem::ptr gem = item->getGemById(gemId);
	if (!gem) {
		printf("Item [%d] has no gem [%d]\n", equipId, gemId);
		return true;
	}
	Dataset::ItemTemplateManager *itm = res->getTemplateManager()->getItemTemplateManager();
	Dataset::ItemTemplate *gemTemp = itm->get(gem->gemId);
	if (!gemTemp) {
		printf("No such gem[%d] template\n", gem->gemId);
		return true;
	}
	if (!g_fragmentTemplate) {
		g_fragmentTemplate= itm->get(Gem::GEM_FRAGMENT_ID);
		if (!g_fragmentTemplate) {
			printf("Get fragment template error, no such\n");
			return true;
		}
	}
	int fragmentCount = role->itemBag()->getItemCountByTemplateId(Gem::GEM_FRAGMENT_ID);
	int error = ErrorCode::ERROR_CODE_NO_ERROR;
	do {
		if (fragmentCount < count) {
			error = ErrorCode::STRENGTH_GEM_NO_ENOUGH_FRAGMENT;
			break;
		}
		gem->exp += g_fragmentTemplate->correspondingExp * count;
		//TODO  Level

	} while (0);

	role->itemBag()->decItemCountByTemplateId(res->getDBConnection(), Gem::GEM_FRAGMENT_ID, count);

	BSON::Object response, body;
	BSON::setStringVal(response, "Command", "StrengGem");
	if (error == ErrorCode::ERROR_CODE_NO_ERROR) {
		BSON::setIntVal(body, "PropId", gem->gemId);
		BSON::setIntVal(body, "Exp", gem->exp);
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
