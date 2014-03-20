#ifndef __ROLE_H__
#define __ROLE_H__

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include "warrior.pb.h"

#include "stats_attr.h"

namespace Windnet {
	namespace Mysql {
		class DBConnection;
	}
}

namespace Dataset {
	class RoleAttribute;
	class RoleAttributeTemplate;
}

class RoleInfo;
class Item;
class ItemBag;
class PlayerSession;
class SoulsBag;
class Scene;
class Task;
class SkillBag;
class Role {
public:
	typedef boost::shared_ptr<Role> ptr;
	Role();
	~Role();

	void loadFromDB(Dataset::RoleAttributeTemplate *ram,
					boost::shared_ptr<Windnet::Mysql::DBConnection>, int roleId);
	void flushToDB(boost::shared_ptr<Windnet::Mysql::DBConnection>);

	boost::shared_ptr<RoleInfo> roleInfo() { return m_roleInfo; }
	void roleInfo(boost::shared_ptr<RoleInfo> &ri) { m_roleInfo = ri; }

	int id() const { return m_role.role_id(); }
	void id(short val) { m_role.set_role_id(val); }

	short profession() const { return m_role.profession(); }
	void profession(short val) { m_role.set_profession(val); }

	int money() const { return m_role.money(); }
	void money(int val) { m_role.set_money(val); }
	void addMoney(int val) {
		int v = money();
		v += val;
		if (v < 0) {
			v = 0;
		}
		m_role.set_money(v);
	}

	int gold() const { return m_role.gold(); }
	void gold(int val) { m_role.set_gold(val); }

	std::string roleName() const { return m_role.role_name(); }
	void roleName(const std::string &val) { m_role.set_role_name(val); }

	short level() const { return m_role.level(); }
	void level(short val) { m_role.set_level(val); }

	short vipLevel() const { return m_role.vip_level(); }
	void vipLevel(short val) { m_role.set_vip_level(val); }

	int exp() const { return m_role.exp(); }
	void exp(int val) { m_role.set_exp(val); }

	int rmb() const { return m_role.rmb(); }
	void rmb(int val) { m_role.set_rmb(val); }

	int lastLogoutTime() { return m_role.last_logout_time(); }
	void lastLogoutTime(int val) { m_role.set_last_logout_time(val); }

	int soulsValue() { return m_role.souls_value(); }
	void soulsValue(int val) { m_role.set_souls_value(val); }
	void addSoulsValue(int val) {
		int now = soulsValue() + val;
		if (now < 0) {
			now = 0;
		}
		m_role.set_souls_value(now);
	}

	short soulsIndex() const { return m_role.souls_index(); }
	void soulsIndex(short val) { m_role.set_souls_index(val); }
	void addSoulsIndex(short val = 1) {
		m_role.set_souls_index(soulsIndex() + val);
	}

	StatsAttr &attr() { return m_attr;}
	void updateAttribute() { }
	void updateAttributeToClient(PlayerSession *ps) { }

	void addEquipItem(boost::shared_ptr<Item> &item) { m_equips.push_back(item); }

	const std::vector<boost::shared_ptr<Item> > & equipments() const { return m_equips; }

	Dataset::RoleAttribute *attribute() { return m_attribute; }

	void scene(Scene *scene) { m_scene = scene; }

	Task* task() { return m_task; }
	Scene *scene() { return m_scene; }
	ItemBag *itemBag() { return m_itemBag; }
	SkillBag *skillBag() { return m_skillBag; }
	SoulsBag* soulsBag() { return m_soulsBag; }

private:
	boost::shared_ptr<RoleInfo> m_roleInfo;
	Scene *m_scene;
	ItemBag *m_itemBag;
	Task *m_task;
	SkillBag *m_skillBag;
	SoulsBag *m_soulsBag;
	Dataset::RoleAttribute *m_attribute;
	std::vector<boost::shared_ptr<Item> > m_equips;
	StatsAttr m_attr;
	int m_fightingValue;
	warrior::Role m_role;
};

#endif
