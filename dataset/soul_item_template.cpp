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
	do {
	}  while (reader2.moveNext());

	return true;
}

SoulHuntInfo* SoulItemTemplate::getSoulHuntInfo(int id) {
	if (id >= 1 && id < (int)m_soulsHuntInfo.size()) {
		return m_soulsHuntInfo[id];
	}
	return NULL;
}

}
