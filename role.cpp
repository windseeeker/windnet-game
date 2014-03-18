#include "role.h"

#include "mysql/dbconnection.h"

#include "dataset/role_attribute_template.h"
#include "item_bag.h"
#include "skill_bag.h"
#include "souls_bag.h"
#include "task.h"

using namespace Windnet;
using namespace Windnet::Mysql;

Role::Role() {
	m_itemBag = new ItemBag;
	m_skillBag = new SkillBag;
	m_task = new Task;
	m_soulsBag = new SoulsBag;
}

void Role::flushToDB(DBConnection::ptr conn) {
	conn->execute("update roles set money=%d, gold=%d, profession=%d, role_lvl=%d, role_exp=%d,role_name='%s',"
				  "vip_lvl=%d, souls_index=%d, souls_value=%d where id=%d", money(), gold(), profession(),
				  level(), exp(), roleName().c_str(), vipLevel(), soulsIndex(), soulsValue(), id());
}

void Role::loadFromDB(Dataset::RoleAttributeTemplate *ram,
					  DBConnection::ptr conn, int roleId) {
	conn->execute("select * from roles where id=%d", roleId);
	size_t rows = conn->rows();

	if (rows == 0) {
		return;
	}
	m_role.set_role_id(roleId);
	m_role.set_money(conn->getInteger(0, "money"));
	m_role.set_gold(conn->getInteger(0, "gold"));
	m_role.set_profession(conn->getInteger(0, "profession"));
	m_role.set_level(conn->getInteger(0, "role_lvl"));
	m_role.set_exp(conn->getInteger(0, "role_exp"));
	m_role.set_role_name(conn->getString(0, "role_name"));
	m_role.set_vip_level(conn->getInteger(0, "vip_lvl"));
	m_role.set_souls_index(conn->getInteger(0, "souls_index"));
	m_role.set_souls_value(conn->getInteger(0, "souls_value"));

	m_attribute = ram->get(level());
	assert(m_attribute);
}
