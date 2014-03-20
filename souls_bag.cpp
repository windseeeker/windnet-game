#include "souls_bag.h"

#include <stdio.h>

#include "mysql/dbconnection.h"

#include "dataset/item_template_manager.h"
#include "util.h"

using namespace Windnet;
using namespace Windnet::Mysql;

SoulItem::SoulItem(Dataset::ItemTemplate *it)
	: m_template(it)  {
	m_soul.set_type(SOUL_ITEM_TYPE_HUNTED);
	m_soul.set_exp(0);
}

short SoulItem::quality() const {
	return m_template->quality;
}

void SoulItem::flushNewToDB(Windnet::Mysql::DBConnection::ptr conn, int roleId) {
	if (conn->execute("insert into souls(soul_id, idx, type, exp, role_id) "
					  "values(%d, %d, %d, %d, %d)", soulId(), index(), type(), exp(), roleId)) {
		id(conn->autoId());
	}
}

void SoulItem::flushToDB(Windnet::Mysql::DBConnection::ptr conn, int roleId) {
	conn->execute("update souls set soul_id=%d, idx=%d, type=%d, exp=%d where id=%d",
				  soulId(), index(), type(), exp(), id());
}

//TODO
short SoulsBag::nextHuntSoulIndex() {
	if (m_huntedSouls.empty()) {
		return 0;
	}
	short size = m_huntedSouls.size();
	short idx[size];
	memset(idx, 0, sizeof(idx[0]) * size);

	for (std::map<int, SoulItem*>::iterator it = m_huntedSouls.begin();
		 it != m_huntedSouls.end(); ++it) {
		if (it->second->index() < size) {
			idx[it->second->index()]++;
		}
	}
	for (int i = 0; i < size; ++i) {
		if (idx[i] == 0) {
			return i;
		}
	}
	return size;
}

void SoulsBag::addSoulItem(SoulItem *item) {
	m_huntedSouls.insert(std::make_pair(item->id(), item));
}

SoulItem *SoulsBag::getSoul(int id) {
	std::map<int, SoulItem*>::iterator it = m_soulsBag.find(id);
	return it == m_soulsBag.end() ? NULL : it->second;
}

SoulItem *SoulsBag::getHuntSoul(int id) {
	std::map<int, SoulItem*>::iterator it  = m_huntedSouls.find(id);
	return it == m_huntedSouls.end() ? NULL : it->second;
}

void SoulsBag::remove(SoulItem *item) {
	m_soulsBag.erase(item->id());
	if (item->index() >= 0 && item->index() < (int)m_soulsVect.size()) {
		m_soulsVect[item->index()] = NULL;
	}
}

//TODO ...
void SoulsBag::removeHuntedSoul(SoulItem *item, bool free) {
	m_huntedSouls.erase(item->id());
	if (free) {
		delete item;
	}
}

void SoulsBag::addEquipSoul(SoulItem *item) {
	m_equippedSouls.insert(std::make_pair(item->id(), item));
}

bool SoulsBag::storeSoulItem(SoulItem *item) {
	size_t idx = 0;
	for (; idx < m_soulsVect.size(); ++idx) {
		if (!m_soulsVect[idx]) {
			break;
		}
	}
	if (idx >= m_soulsVect.size()) {
		return false;
	}
	item->type(SoulItem::SOUL_ITEM_TYPE_IN_BAG);
	item->index(idx);
	m_huntedSouls.erase(item->id());
	m_soulsVect[idx] = item;
	m_soulsBag[item->id()] = item; /////////////////////////////////Problem ???????
	return true;
}

void SoulsBag::loadFromDB(Dataset::ItemTemplateManager *itm, DBConnection::ptr conn, int roleId) {
	conn->execute("select * from souls where role_id = %d", roleId);
	size_t rows = conn->rows();

	m_soulsVect.resize(SOULS_COUNT_PER_PAGE * PAGE_COUNT, 0);
	printf("Role %d has %d soulss\n", roleId, (int)rows);
	for (size_t i = 0; i < rows; ++i) {
		int soulId = conn->getInteger(i, "soul_id");
		Dataset::ItemTemplate *it = itm->get(soulId);
		if (!it) {
			printf("No such soul id %d\n", soulId);
			continue;
		}
		int type = conn->getSmallint(i, "type");
		if (type == SoulItem::SOUL_ITEM_TYPE_DELETED) {
			continue;
		}
		SoulItem *item = new SoulItem(it);
		item->soulId(soulId);
		item->id(conn->getInteger(i, "id"));
		item->index(conn->getSmallint(i, "idx"));
		item->type(type);
		item->exp(conn->getInteger(i, "exp"));

		if (item->type() == SoulItem::SOUL_ITEM_TYPE_HUNTED) {
			m_huntedSouls.insert(std::make_pair(item->id(), item));
		} else if (item->type() == SoulItem::SOUL_ITEM_TYPE_IN_BAG) {
			m_soulsBag.insert(std::make_pair(item->id(), item));

			if (item->index() >= (int)m_soulsVect.size()) {
				printf("Invalid soul item idx %d\n", item->index());
				continue;
			}
			m_soulsVect[item->index()] = item;
		} else {
			m_equippedSouls.insert(std::make_pair(item->id(), item));
		}
	}
}
