#ifndef __SOUL_ITEM_TEMPLATE_H__
#define __SOUL_ITEM_TEMPLATE_H__

#include <boost/shared_ptr.hpp>

#include <map>
#include <vector>

namespace Dataset {
struct SoulHuntInfo {
	int id;
	int moneyNeed;
	int levelNeed;
	int soulValue;
	double levelUpRate;
	std::map<short, short> randQuality;
};

class SoulItemTemplate {
public:
	typedef boost::shared_ptr<SoulItemTemplate> ptr;

	bool init();

	SoulHuntInfo *getSoulHuntInfo(int id);

private:
	std::vector<SoulHuntInfo*> m_soulsHuntInfo;
	std::map<int, SoulItemTemplate::ptr> m_items;
};
}

#endif
