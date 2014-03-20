#ifndef __SOULS_BAG_H__
#define __SOULS_BAG_H__

#include <boost/shared_ptr.hpp>

#include <map>
#include <list>
#include <vector>

#include "warrior.pb.h"

namespace Windnet {
	namespace Mysql {
		class DBConnection;
	}
}

namespace Dataset {
	class ItemTemplate;
	class ItemTemplateManager;
}

class SoulItem {
public:
	SoulItem(Dataset::ItemTemplate *it);

	enum {
		SOUL_ITEM_TYPE_HUNTED = 0,
		SOUL_ITEM_TYPE_IN_BAG = 1,
		SOUL_ITEM_TYPE_EQUIPPED = 2,
		SOUL_ITEM_TYPE_DELETED = 100
	};

	int id() const { return m_soul.id(); }
	void id(int val) { m_soul.set_id(val); }

	int soulId() const { return m_soul.soul_id(); }
	void soulId(int val) { m_soul.set_soul_id(val); }

	short index() const { return m_soul.index(); }
	void index(short val) { return m_soul.set_index(val); }

	short quality() const;

	short type() const { return m_soul.type(); }
	void type(short val) { m_soul.set_type(val); }

	int exp() const { return m_soul.exp(); }
	void exp(int val) { m_soul.set_exp(val); }
	void addExp(int val) {
		int expVal = exp() + val;
		if (expVal < 0) {
			expVal = 0;
		}
		m_soul.set_exp(expVal);
	}

	Dataset::ItemTemplate *getTemplate() { return m_template; }
	void setTemplate(Dataset::ItemTemplate *it) { m_template = it; }

	void flushNewToDB(boost::shared_ptr<Windnet::Mysql::DBConnection>, int roleId);
	void flushToDB(boost::shared_ptr<Windnet::Mysql::DBConnection>, int roleId);

private:
	warrior::SoulItem m_soul;
	Dataset::ItemTemplate *m_template;
};

class SoulsBag {
public:
	static const size_t MAX_HUNT_SOULS_COUNT = 18;
	static const int SOULS_COUNT_PER_PAGE = 12;
	static const int PAGE_COUNT = 4;

	SoulsBag() {};

	void loadFromDB(Dataset::ItemTemplateManager *itm,
					boost::shared_ptr<Windnet::Mysql::DBConnection>, int roleId);

	short nextHuntSoulIndex();
	void addSoulItem(SoulItem *item);

	bool storeSoulItem(SoulItem *item);

	SoulItem *getHuntSoul(int id);
	SoulItem *getSoul(int id);

	void addEquipSoul(SoulItem *item);
	void remove(SoulItem *item);
	void removeHuntedSoul(SoulItem *item, bool free = true);

	const std::map<int, SoulItem*> & soulsBag() const { return m_soulsBag; }
	const std::map<int, SoulItem*> & equippedSouls() const { return m_equippedSouls; }

	std::map<int, SoulItem*> huntedSouls() const { return m_huntedSouls; }

	bool isFull() { return m_huntedSouls.size() >= MAX_HUNT_SOULS_COUNT; }

private:
	std::map<int, SoulItem*> m_huntedSouls;

	std::map<int, SoulItem*> m_soulsBag;
	std::vector<SoulItem*> m_soulsVect;

	std::map<int, SoulItem*> m_equippedSouls;
};

#endif
