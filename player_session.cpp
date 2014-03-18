#include "player_session.h"

#include "mysql/dbconnection.h"

#include "account_manager.h"
#include "dataset/role_attribute_template.h"
#include "dataset/item_template_manager.h"
#include "game_template_manager.h"
#include "server_resource.h"
#include "scene.h"
#include "role.h"
#include "item_bag.h"
#include "souls_bag.h"
#include "util.h"

using namespace Windnet;
using namespace Mysql;

RoleInfo::ptr PlayerSession::roleInfo() {
	return m_role ? m_role->roleInfo() : RoleInfo::ptr();
}

bool PlayerSession::loadRole(ServerResource::ptr res, int roleId, boost::shared_ptr<RoleInfo> &ri) {
	if (m_role) {
		return true;
	}
	m_role = new Role;
	m_role->roleInfo(ri);

	Dataset::ItemTemplateManager *itm = res->getTemplateManager()->getItemTemplateManager();
	m_role->loadFromDB(res->getTemplateManager()->getRoleAttriTemplate(), res->getDBConnection(), roleId);
	m_role->itemBag()->loadFromDB(itm, res->getDBConnection(), roleId);
	m_role->soulsBag()->loadFromDB(itm, res->getDBConnection(), roleId);
	return true;
}

void PlayerSession::close() {
	Socket::close();
	playerOffline();
}

void PlayerSession::playerOnline() {
	printf("Player online\n");
}

void PlayerSession::playerOffline() {
	if (!m_role) {
		return;
	}
	printf("Player offline\n");
	if (m_role->scene()) {
		m_role->scene()->leaveScene(this);
	}
	m_role->lastLogoutTime(getCurrentSecond());
}
