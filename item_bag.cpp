#include "item_bag.h"
#include "item.h"

#include <stdio.h>
#include <boost/lexical_cast.hpp>

#include "mysql/dbconnection.h"

#include "dataset/item_template_manager.h"
#include "role.h"
#include "util.h"

using namespace Windnet;
using namespace Windnet::Mysql;

bool ItemCompare(Item::ptr item1, Item::ptr item2) {
	Dataset::ItemTemplate *it1 = item1->getTemplate();
	Dataset::ItemTemplate *it2 = item2->getTemplate();
	return it1->itemType != it2->itemType ? it1->itemType < it2->itemType : item1->id() < item2->id();
}

void ItemBag::sortItems() {
	if (m_lastSyncTime + 3 > getCurrentSecond()) {
		return;
	}
	m_lastSyncTime = getCurrentSecond();
	std::vector<Item::ptr> items;
	for (size_t i = 1; i < m_items.size(); ++i) {
		if (m_items[i]) {
			items.push_back(m_items[i]);
		}
	}
	std::sort(items.begin(), items.end(), ItemCompare);
	m_items.clear();
	m_items.resize(1);

	size_t i = 0;
	for (; i < items.size(); ++i) {
		if (items[i]->number() <= 0) {
			continue;
		}
		Dataset::ItemTemplate *it = items[i]->getTemplate();
		if (it->stackNum <= 1 || items[i]->number() >= it->stackNum) {
			m_items.push_back(items[i]);
			continue;
		}
		Item::ptr back = m_items.back();
		if (items[i]->itemId() != back->itemId() ||
			back->number() >= back->getTemplate()->stackNum) {
			m_items.push_back(items[i]);
			continue;
		}
		if (back->number() + items[i]->number() <= back->getTemplate()->stackNum) {
			back->addNum(items[i]->number());
			items[i]->number(0);
		} else {
			items[i]->addNum(back->number() - back->getTemplate()->stackNum);
			back->number(back->getTemplate()->stackNum);
			--i;
		}
	}
	for (size_t i = 1; i < m_items.size(); ++i) {
		m_items[i]->index(i);
	}
	m_items.resize(DEFAULT_BAG_CELL);
}

bool ItemBag::stackItem(Item::ptr &item) {
	short stackNum = item->getTemplate()->stackNum;
	if (stackNum == 0) {
		return false;
	}
	for (size_t i = 0; i < m_items.size(); ++i) {
		if (item->number() <= 0 || !m_items[i]) {
			continue;
		}
		if (m_items[i]->itemId() != item->itemId()) {
			continue;
		}
		if (m_items[i]->number() + item->number() <= stackNum) {
			m_items[i]->addNum(item->number());
			item->number(0);
			break;
		}
		item->addNum(m_items[i]->number() - stackNum);
		m_items[i]->number(stackNum);
	}
	return true;
}

void ItemBag::removeItem(Item::ptr &item, bool resideInMap) {
	if (item->index() >= 1 && item->index() < (short)m_items.size()) {
		m_items[item->index()].reset();
	}
	if (!resideInMap) {
		m_itemMap.erase(item->id());
	}
}

void ItemBag::addItem(Item::ptr &item) {
	size_t idx = 1;
	for (; idx < m_items.size(); ++idx) {
		if (!m_items[idx]) {
			break;
		}
	}
	item->index(idx);
	if (idx >= m_items.size()) {
		m_items.resize(idx + 1);
	}
	m_items[idx] = item;
	m_itemMap[item->id()] = item;
}

void ItemBag::moveItem(DBConnection::ptr conn, int fromId, short toIdx) {
	if (toIdx < 1 || toIdx >= (short)m_items.size()) {
		printf("Invalid move item to idx%d\n", toIdx);
		return;
	}
	std::map<int, Item::ptr>::iterator it = m_itemMap.find(fromId);
	if (it == m_itemMap.end()) {
		printf("No such item to move %d\n", fromId);
		return;
	}
	Item::ptr fromItem = it->second;
	short fromIdx = fromItem->index();
	Item::ptr toItem = m_items[toIdx];
	if (!toItem || fromItem->itemId() != toItem->itemId() || fromItem->getTemplate()->stackNum == 0) {
		std::swap(m_items[fromIdx], m_items[toIdx]);
		fromItem->index(toIdx);
		if (toItem) {
			toItem->index(fromIdx);
			toItem->flushToDB(conn);
		}
		fromItem->flushToDB(conn);
		return;
	}
	Dataset::ItemTemplate *itemTemp = fromItem->getTemplate();
	if (toItem->number() + fromItem->number() <= itemTemp->stackNum) {
		toItem->addNum(fromItem->number());
		fromItem->number(0);
		removeItem(fromItem);
	} else {
		fromItem->addNum(toItem->number() - itemTemp->stackNum);
		toItem->number(itemTemp->stackNum);
	}
	fromItem->flushToDB(conn);
	if (toItem) {
		toItem->flushToDB(conn);
	}
}

void ItemBag::listItems(std::list<Item::ptr> &l) {
	for (size_t i = 0; i < m_items.size(); ++i) {
		if (m_items[i]) {
			l.push_back(m_items[i]);
		}
	}
}

Item::ptr ItemBag::get(int itemId) {
	std::map<int, Item::ptr>::iterator it = m_itemMap.find(itemId);
	return it == m_itemMap.end() ? Item::ptr() : it->second;
}

//TODO ...............Optimize
int ItemBag::getItemCountByTemplateId(int templateId) {
	int ret = 0;
	for (std::map<int, Item::ptr>::iterator it = m_itemMap.begin();
		 it != m_itemMap.end(); ++it) {
		if (it->second->itemId() == templateId) {
			++ret;
		}
	}
	return ret;
}

void ItemBag::loadFromDB(Dataset::ItemTemplateManager *itm, DBConnection::ptr conn, Role *role) {
	conn->execute("select * from items where role_id = %d", role->id());
	size_t rows = conn->rows();

	printf("Role %d has %d items\n", role->id(), (int)rows);
	for (size_t i = 0; i < rows; ++i) {
		int templateId = conn->getInteger(i, "item_id");
		Dataset::ItemTemplate *it = itm->get(templateId);
		if (!it) {
			continue;
		}
		int state = conn->getSmallint(i, "state");
		if (state != Item::ITEM_STATE_NORMAL) {
			continue;
		}
		Item::ptr item(new Item(it));
		item->id(conn->getInteger(i, "id"));
		item->roleId(role->id());
		item->itemId(conn->getInteger(i, "item_id"));
		item->number(conn->getInteger(i, "number"));
		item->index(conn->getInteger(i, "item_idx"));
		std::string gemStr = conn->getString(i, "gems");
		if (!gemStr.empty()) {
			std::vector<std::string> vect = split(gemStr, ';');
			for (size_t j = 0; j < vect.size(); ++j) {
				std::vector<std::string> gemVect = split(vect[j], ',');
				if (gemVect.size() != 4) {
					printf("Boad gem stringggggggggggggg in db\n");
					continue;
				}
				Gem::ptr gem(new Gem);
				gem->id = boost::lexical_cast<int>(gemVect[0]);
				gem->gemId = boost::lexical_cast<int>(gemVect[1]);
				gem->pos = boost::lexical_cast<int>(gemVect[2]);
				gem->exp = boost::lexical_cast<int>(gemVect[3]);

				item->m_gems.push_back(gem);;
			}
		}
		item->isEquip(conn->getSmallint(i, "is_equip"));
		if (item->isEquip()) {
			role->addEquipItem(item);
		} else {
			if (item->index() >= (int)m_items.size()) {
				m_items.resize(item->index() + 1);
			}
			m_items[item->index()] = item;
		}
		m_itemMap.insert(std::make_pair(item->id(), item));
	}
}
