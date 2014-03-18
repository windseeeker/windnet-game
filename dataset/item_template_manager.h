#ifndef __ITEM_TEMPLATE_MANAGER_H__
#define __ITEM_TEMPLATE_MANAGER_H__

#include <map>
#include <vector>

namespace Dataset {
class RoleAttribute;

enum ItemType {
	ITEM_TYPE_EQUIP = 1,
	ITEM_TYPE_CURE = 2,
	ITEM_TYPE_GEM = 3,
	ITEM_TYPE_FRAGMENT = 4,
	ITEM_TYPE_PET = 5,
	ITEM_TYPE_MOUNT = 6,
	ITEM_TYPE_CONSUME = 7,
	ITEM_TYPE_BAG = 8,
	ITEM_TYPE_TASK = 9,
	ITEM_TYPE_SOUL = 10
};

 enum {
	 ITEM_QUALITY_WHITE = 1,
	 ITEM_QUALITY_GREEN = 2,
	 ITEM_QUALITY_BLUE = 3,
	 ITEM_QUALITY_PURPLE = 4,
	 ITEM_QUALITY_RED = 5,
	 ITEM_QUALITY_ORANGE = 6,
	 ITEM_QUALITY_COUNT
 };

struct ItemTemplate {
	int itemId;
	int price;
	short equipType;
	short itemType;
	short itemUseLevel;
	char profession;
	short quality;
	short stackNum;
	short strengthNextLevelId;
	short strengthLevel;
	RoleAttribute *attr;
};

class ItemTemplateManager {
public:
	bool init();

	ItemTemplate * get(int id);

	ItemTemplate *getRandomSoulItemByQuality(short quality);
	int getStrengthEquipMoney(short &quality, short &level);

private:
	std::map<int, ItemTemplate*> m_items;
	std::map<int, std::vector<int> > m_equipStrength;

	//soulItem Random set
	std::map<short, std::vector<ItemTemplate*> > m_soulItems;
};
}

#endif
