#include "item_template_manager.h"

#include "role_attribute_template.h"
#include "../assert.h"
#include "../util.h"

#include "csv_reader.h"

namespace Dataset {
bool ItemTemplateManager::init() {
	CSVReader reader;
	if (!reader.loadFile("./data/items.csv")) {
		ASSERT(false);
	}
	do {
		ItemTemplate *item = new ItemTemplate;
		ASSERT(reader.Attribute("Item_id", item->itemId));
		ASSERT(reader.Attribute("Item_Level", item->itemUseLevel));
		ASSERT(reader.Attribute("Item_Type", item->itemType));
		ASSERT(reader.Attribute("Item_ZL", item->profession));
		ASSERT(reader.Attribute("Item_LevelType", item->quality));
		ASSERT(reader.Attribute("Item_Sex", item->stackNum));
		reader.Attribute("StrengThen_Next", item->strengthNextLevelId); ///////
		ASSERT(reader.Attribute("strengthenLevel", item->strengthLevel));

		RoleAttribute *ra = new RoleAttribute;
		//ASSERT(reader.Attribute("Maxhp", ra->maxHP));
		//ASSERT(reader.Attribute("Maxmp", ra->maxMP));
		//ASSERT(reader.Attribute("minAttack", ra->minPhysicalAttack));
		//ASSERT(reader.Attribute("maxAttack", ra->maxPhysicalAttack));
		//ASSERT(reader.Attribute("minMagic", ra->maxMagicAttack));
		//ASSERT(reader.Attribute("maxMagic", ra->minMagicAttack));
		//ASSERT(reader.Attribute("Defense", ra->defence));
		//ASSERT(reader.Attribute("MagicDefense", ra->magicDefence));

		//ASSERT(reader.Attribute("Item_Hits", ra->hit));
		//ASSERT(reader.Attribute("Attack_Fatal", ra->crit));
		item->attr = ra;
		m_items.insert(std::make_pair(item->itemId, item));

		//soulItem
		if (item->itemType == ITEM_TYPE_SOUL) {
			std::map<short, std::vector<ItemTemplate*> >::iterator it = m_soulItems.find(item->quality);
			if (it != m_soulItems.end()) {
				(it->second).push_back(item);
			} else {
				std::vector<ItemTemplate*> vect;
				vect.push_back(item);
				m_soulItems[item->quality] = vect;
			}
		}
	}  while (reader.moveNext());

	CSVReader reader2;
	if (!reader2.loadFile("./data/equip_strength.csv")) {
		ASSERT(false);
	}
	do {
		int quality, level, money;
		ASSERT(reader2.Attribute("Item_LevelType", quality));
		ASSERT(reader2.Attribute("leve", level));
		ASSERT(reader2.Attribute("yinbi", money));

		std::vector<int> *vect;
		std::map<int, std::vector<int> >::iterator it = m_equipStrength.find(quality);
		if (it == m_equipStrength.end()) {
			std::vector<int> v;
			m_equipStrength.insert(std::make_pair(quality, v));

			it = m_equipStrength.find(quality);
		}
		vect = &(it->second);
		if ((int)vect->size() <= level) {
			vect->resize(level + 1);
		}
		vect->at(level) = money;
	}  while (reader2.moveNext());

	return true;
}

ItemTemplate *ItemTemplateManager::get(int id) {
	std::map<int, ItemTemplate*>::iterator it = m_items.find(id);
	return it == m_items.end() ? NULL : it->second;
}

int ItemTemplateManager::getStrengthEquipMoney(short &quality, short &level) {
	if (quality == 0) {
		quality = 1;
		level = 1;
	}
	std::map<int, std::vector<int> >::iterator it = m_equipStrength.find(quality);
	if (it == m_equipStrength.end()) {
		return -1;
	}

	std::vector<int> &vect = it->second;
	if (level + 1 >= (int)vect.size()) {
		level = 1;

		it = m_equipStrength.find(++quality);
		if (it == m_equipStrength.end()) {
			return -1;
		}
		std::vector<int> &v = it->second;
		return v.size() > 1 ? v[1] : -1;
	}
	return vect.at(++level);
}

ItemTemplate *ItemTemplateManager::getRandomSoulItemByQuality(short quality) {
	std::map<short, std::vector<ItemTemplate*> >::iterator it = m_soulItems.find(quality);
	if (it == m_soulItems.end()) {
		return NULL;
	}
	std::vector<ItemTemplate*> &vect = it->second;
	return vect.empty() ? NULL : vect[randInt(0, vect.size())];
}
}
