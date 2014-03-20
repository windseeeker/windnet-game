#ifndef __ITEM_H__
#define __ITEM_H__

#include <boost/shared_ptr.hpp>

#include "warrior.pb.h"

#include "stats_attr.h"

namespace Windnet {
	namespace Mysql {
		class DBConnection;
	}
}

namespace Dataset {
	class ItemTemplate;
}

struct Gem {
	typedef boost::shared_ptr<Gem> ptr;

	const static int GEM_FRAGMENT_ID = 21000;
	int id;
	int gemId;
	char pos;
	int exp;
};

class ItemBag;
class Item {
public:
	friend class ItemBag;
	typedef boost::shared_ptr<Item> ptr;

	enum {
		ITEM_STATE_NORMAL = 0,
		ITEM_STATE_DELETED = 100
	};

    Item(Dataset::ItemTemplate *it):m_template(it) { }

	int id() const { return m_item.global_id(); }
	void id(int val) { m_item.set_global_id(val); }

	int itemId() const { return m_item.item_id(); }
	void itemId(int val) { m_item.set_item_id(val); }

	short level() const { return m_item.level(); }
	void level(short val) { m_item.set_level(val); }

	short quality() const { return m_item.quality(); }
	void quality(short val) { m_item.set_quality(val); }

	int roleId() const { return m_item.role_id(); }
	void roleId(int val) { m_item.set_role_id(val); }

	short number() const { return m_item.item_num(); }
	void number(short val) { m_item.set_item_num(val); }
	void addNum(short val) { m_item.set_item_num(number() + val); }

	short index() const { return m_item.index(); }
	void index(short val) { m_item.set_index(val); }

	bool isEquip() const { return m_item.is_equip(); }
	void isEquip(short val) { m_item.set_is_equip(val); }

	short state() const { return m_item.state(); }
	void state(short val) { m_item.set_state(val); }

	const std::vector<Gem::ptr> gems() { return m_gems; }
	bool addGem(Gem::ptr &gem);

	Gem::ptr getGemById(int dbid);

	Dataset::ItemTemplate *getTemplate() { return m_template; }
	void setTemplate(Dataset::ItemTemplate *it) { m_template = it; }

	static Item::ptr createItem(Dataset::ItemTemplate *it, int roleId, short itemNum);

	void flushNewToDB(boost::shared_ptr<Windnet::Mysql::DBConnection> conn); ////TODB remove in future
	void flushToDB(boost::shared_ptr<Windnet::Mysql::DBConnection> conn);

private:
	Dataset::ItemTemplate *m_template;
	warrior::Item m_item;
	std::vector<Gem::ptr> m_gems;
	StatsAttr m_attr;
};

#endif
