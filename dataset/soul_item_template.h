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

struct SoulInfo {
	short quality;
	short level;
	int exp;
	int swallowExp;
};

class SoulItemTemplate {
public:
	typedef boost::shared_ptr<SoulItemTemplate> ptr;
	typedef std::pair<std::vector<SoulInfo*>, std::map<int, SoulInfo*> > soulsInfoPair;

	bool init();

	SoulHuntInfo *getSoulHuntInfo(short id);
	SoulInfo *getSoulInfo(short quality, short level);
	short getLevelByExp(short quality, int exp);

	int getSoulIdBySoulValue(int value);

private:
	std::vector<SoulHuntInfo*> m_soulsHuntInfo;
	std::vector<soulsInfoPair> m_soulsInfo;

	std::map<int, int> m_soulsExchange;
};
}

#endif
