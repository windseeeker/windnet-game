#ifndef __STATSATTRI_H__
#define __STATSATTRI_H__

#include <map>
#include <string>

class StatsAttr {
public:
	enum AttriType{
		AT_MAX_HP = 1,
		AT_HP = 2,
		AT_MAX_MP = 3,
		AT_MP = 4,
		AT_MAX_PHYSICAL_ATTACK = 5,
		AT_MIN_PHYSICAL_ATTACK = 6,
		AT_MAX_MAGIC_ATTACK = 7,
		AT_MIN_MAGIC_ATTACK = 8,
		AT_DEFENCE = 9,
		AT_HTI = 10,
		AT_MAGIC_DEFENCE = 11,
		AT_DODEG = 12,
		AT_CRIT = 13,
		AT_TOUGHNESS = 14,
		AT_MOVE_SPEED = 15,
		AT_AGGRESSIVITY = 16,
		AT_COUNT
	};

	void addStatAttr(short type, double val);
	void updateStatAttr(short type, double val);

	double getStatAttr(short type);

private:
	std::map<short, double> m_statsAttr;
};

#endif
