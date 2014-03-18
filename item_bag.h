#ifndef __ITEM_BAG_H__
#define __ITEM_BAG_H__

#include <boost/shared_ptr.hpp>

#include <map>
#include <list>
#include <vector>

namespace Windnet {
	namespace Mysql {
		class DBConnection;
	}
}

namespace Dataset {
	class ItemTemplateManager;
}

class Item;
class ItemBag {
public:
	typedef boost::shared_ptr<ItemBag> ptr;
	static const int DEFAULT_BAG_CELL = 25;

     ItemBag():m_lastSyncTime(0) { m_items.resize(DEFAULT_BAG_CELL); }

	void loadFromDB(Dataset::ItemTemplateManager *item,
					boost::shared_ptr<Windnet::Mysql::DBConnection>, int roleId);

	void listItems(std::list<boost::shared_ptr<Item> > &l);
	const std::vector<boost::shared_ptr<Item> > &items() const { return m_items; }

	bool isFull() { return false; }

	boost::shared_ptr<Item> get(int id);

	bool stackItem(boost::shared_ptr<Item> &item);
	void addItem(boost::shared_ptr<Item> &item);
	void removeItem(boost::shared_ptr<Item> &item, bool resideInMap = false);

	void sortItems();
	void moveItem(boost::shared_ptr<Windnet::Mysql::DBConnection> conn, int fromId, short toIdx);

	int getItemCountByTemplateId(int itemId) { return 1; }
	void decItemCountByTemplateId(int itemId, short count) { }

private:
	unsigned int m_lastSyncTime;
	std::map<int, boost::shared_ptr<Item> > m_itemMap;
	std::vector<boost::shared_ptr<Item> > m_items;
};

#endif
