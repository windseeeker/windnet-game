#include "scene.h"

#include "buffer.h"

#include "bson/bson_parser.h"
#include "bson_buffer.h"

#include "account_manager.h" ////////////////TODB should be removed in future
#include "role.h"
#include "player_session.h"

using namespace Windnet;

void Scene::addPlayer(int id, PlayerSession *ps) {
	m_players.insert(std::make_pair(id, ps));
}

void Scene::removePlayer(int roleId) {
	m_players.erase(roleId);
}

void Scene::sendSurrounding(int id, PlayerSession *ps) {
	BSON::Object notify, body;
	BSON::Array array;
	BSON::setStringVal(notify, "Command", "SameUsers");

	std::map<int, PlayerSession*>::iterator it = m_players.begin();
	for (; it != m_players.end(); ++it) {
		if (it->first == id) {
			continue;
		}
		printf("Load surrounding %s \n", it->second->roleInfo()->roleName.c_str());
		BSON::Object obj;
		BSON::setIntVal(obj, "Id", it->second->roleInfo()->roleId);
		BSON::setIntVal(obj, "ProfessionId", it->second->roleInfo()->profession);
		BSON::setStringVal(obj, "Name", it->second->roleInfo()->roleName);
		BSON::setIntVal(obj, "Level", it->second->roleInfo()->level);
		BSON::setDoubleVal(obj, "X", it->second->roleInfo()->posX);
		BSON::setDoubleVal(obj, "Y", it->second->roleInfo()->posY);
		//obj.setIntValue("Pet", it->second->roleInfo()->roleId);
		//obj.setIntValue("Mounts", it->second->roleInfo()->roleId);
		BSON::Value val;
		val = obj;
		val.valueType = BSON_OBJECT;
		array.push_back(val);
	}
	BSON::setArrayVal(body, "SameUsers", array);
	BSON::setObjectVal(notify, "Body", body);

	Buffer buf;
	BSON::serializeToBuffer(buf, notify);
	BsonBuffer::serializeToBuffer(buf);
	ps->sendData(buf);
}

void Scene::leaveScene(PlayerSession *ps) {
	printf("role %d leaves scene\n", ps->role()->id());
	removePlayer(ps->role()->id());

	BSON::Object notify, body;
	BSON::setStringVal(notify, "Command", "OtherLeaveLobby");
	BSON::setIntVal(body, "UserId", ps->role()->id());
	BSON::setObjectVal(notify, "Body", body);
	Buffer buf;
	BSON::serializeToBuffer(buf, notify);
	BsonBuffer::serializeToBuffer(buf);
	broadcast(buf);
}

void Scene::broadcast(Windnet::Buffer &buf, int selfRoleId) {
	std::map<int, PlayerSession*>::iterator it = m_players.begin();
	for (; it != m_players.end(); ++it) {
		if (selfRoleId == it->first) {
			for (++it; it != m_players.end(); ++it) {
				it->second->sendData(buf);
			}
			break;
		}
		it->second->sendData(buf);
	}
}
