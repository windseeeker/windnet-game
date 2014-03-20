#include "item.h"

#include <sstream>

#include "mysql/dbconnection.h"

#include "dataset/item_template_manager.h"

using namespace Windnet;
using namespace Windnet::Mysql;

Item::ptr Item::createItem(Dataset::ItemTemplate *it, int roleId, short itemNum) {
	Item::ptr item(new Item(it));
	item->itemId(it->itemId);
	item->roleId(roleId);
	item->number(itemNum);
	return item;
}

bool Item::addGem(Gem::ptr &gem) {
	if (gem->pos >= 0 && gem->pos < (int)m_gems.size() && m_gems[gem->pos]) {
		return false;
	}
	if (gem->pos >= (int)m_gems.size()) {
		m_gems.resize(gem->pos + 1);
	}
	m_gems[gem->pos] = gem;
	return true;
}

Gem::ptr Item::getGemById(int dbid) {
	for (size_t i = 0; i < m_gems.size(); ++i) {
		if (m_gems[i] && m_gems[i]->id == dbid) {
			return m_gems[i];
		}
	}
	return Gem::ptr();
}

void Item::flushNewToDB(DBConnection::ptr conn) {
	bool b = conn->execute("insert into items(role_id, item_id, number, item_idx) values(%d, %d, %d, %d)",
						   roleId(), itemId(), number(), index());
	if (b) {
		id(conn->autoId());
	}
}

void Item::flushToDB(DBConnection::ptr conn) {
	if (number() == 0) {
		state(ITEM_STATE_DELETED);
	}
	std::stringstream ss;
	for (size_t i = 0; i < m_gems.size(); ++i) {
		ss << m_gems[i]->id << ',' << m_gems[i]->gemId << ',' << m_gems[i]->pos << ','
		   << m_gems[i]->exp;
	}
	std::string str = ss.str();
	if (!str.empty()) {
		str.resize(str.size() - 1);
	}
	conn->execute("update items set role_id=%d, number=%d, item_idx=%d, state=%d, level=%d, quality=%d, gems='%s' "
				  "where id=%d", roleId(), number(), index(), state(), level(), quality(), str.c_str(), id());
}

