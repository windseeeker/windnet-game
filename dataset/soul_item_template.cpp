#include "soul_item_template.h"

#include "../assert.h"

#include "csv_reader.h"
#include "item_template_manager.h"

namespace Dataset {
bool SoulItemTemplate::init() {
	CSVReader reader;
	if (!reader.loadFile("./data/souls_hunt.csv")) {
		ASSERT(false);
	}
	int val;
	do {
		SoulHuntInfo * info = new SoulHuntInfo;
		ASSERT(reader.Attribute("Id", info->id));
		ASSERT(reader.Attribute("Money", info->moneyNeed));
		ASSERT(reader.Attribute("LevelNeed", info->levelNeed));
		ASSERT(reader.Attribute("LevelUpRate", info->levelUpRate));
		ASSERT(reader.Attribute("SoulValue", info->soulValue));

		int weight = 0;
		ASSERT(reader.Attribute("White", val));
		weight += val;
		info->randQuality[weight] = ITEM_QUALITY_WHITE;

		ASSERT(reader.Attribute("Green", val));
		weight += val;
		info->randQuality[weight] = ITEM_QUALITY_GREEN;

		ASSERT(reader.Attribute("Blue", val));
		weight += val;
		info->randQuality[weight] = ITEM_QUALITY_BLUE;

		ASSERT(reader.Attribute("Purple", val));
		weight += val;
		info->randQuality[weight] = ITEM_QUALITY_PURPLE;

		ASSERT(reader.Attribute("Orange", val));
		weight += val;
		info->randQuality[weight] = ITEM_QUALITY_ORANGE;

		ASSERT(reader.Attribute("Red", val));
		weight += val;
		info->randQuality[weight] = ITEM_QUALITY_RED;

		if (info->id >= (int)m_soulsHuntInfo.size()) {
			m_soulsHuntInfo.resize(info->id + 1);
		}
		m_soulsHuntInfo[info->id] = info;
	}  while (reader.moveNext());

	CSVReader reader2;
	if (!reader2.loadFile("./data/souls_exp.csv")) {
		ASSERT(false);
	}
	int type, level, exp, swallowExp;
	do {
		ASSERT(reader2.Attribute("Type", type));
		ASSERT(reader2.Attribute("Level", level));
		ASSERT(reader2.Attribute("Exp", exp));
		ASSERT(reader2.Attribute("SwallowExp", swallowExp));

		if (type >= (int)m_soulsInfo.size()) {
			m_soulsInfo.resize(type + 1);
			soulsInfoPair pair;
			m_soulsInfo[type] = pair;
		}
		std::vector<SoulInfo*> &vect = m_soulsInfo[type].first;

		if (level >= (int)vect.size()) {
			vect.resize(level + 1, 0);
		}
		SoulInfo *info = vect[level];
		if (!info) {
			info = new SoulInfo;
			vect[level] = info;
		}
		info->quality = type;
		info->level = level;
		info->exp = exp;
		info->swallowExp = swallowExp;

		std::map<int, SoulInfo*> &expMap = m_soulsInfo[type].second;
		std::map<int, SoulInfo*>::iterator it = expMap.find(exp);
		if (it == expMap.end()) {
			expMap[exp] = info;
		}
	}  while (reader2.moveNext());

	CSVReader reader3;
	if (!reader3.loadFile("./data/souls_shop.csv")) {
		ASSERT(false);
	}
	do {
		int id, value;
		ASSERT(reader3.Attribute("SoulMetaId", id));
		ASSERT(reader3.Attribute("SoulPoint", value));

		m_soulsExchange[value] = id;
	}  while (reader2.moveNext());

	return true;
}

SoulHuntInfo* SoulItemTemplate::getSoulHuntInfo(short id) {
	if (id >= 1 && id < (short)m_soulsHuntInfo.size()) {
		return m_soulsHuntInfo[id];
	}
	return NULL;
}

SoulInfo* SoulItemTemplate::getSoulInfo(short quality, short level) {
	if (quality < 1 && quality >= (short)m_soulsInfo.size()) {
		return NULL;
	}
	std::vector<SoulInfo*> &vect = m_soulsInfo[quality].first;
	if (level < 1 && level >= (short)vect.size()) {
		return NULL;
	}
	return vect[level];
}

short SoulItemTemplate::getLevelByExp(short quality, int exp) {
	if (quality < 1 && quality >= (short)m_soulsInfo.size()) {
		return -1;
	}
	std::map<int, SoulInfo*> &expMap = m_soulsInfo[quality].second;
	std::map<int, SoulInfo*>::iterator it = expMap.lower_bound(exp);
	if (it == expMap.end()) {
		return -1;
	}
	return it->second ? it->second->level : -1;
}

int SoulItemTemplate::getSoulIdBySoulValue(int value) {
	std::map<int, int>::iterator it = m_soulsExchange.find(value);
	return it == m_soulsExchange.end() ? -1 : it->second;
}
}
