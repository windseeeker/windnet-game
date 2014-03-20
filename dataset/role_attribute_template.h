#ifndef __ROLE_ATTRIBUTE_TEMPLATE_H__
#define __ROLE_ATTRIBUTE_TEMPLATE_H__

#include <boost/shared_ptr.hpp>

#include <map>
#include <vector>

namespace Dataset {
struct RoleAttribute {
	int levelExp;
	int maxHP;
	int hp;
	int maxMP;
	int mp;
	int minPhysicalAttack;
	int maxPhysicalAttack;
	int maxMagicAttack;
	int minMagicAttack;
	int defence;
	int magicDefence;
	int hit;
	int dodge;
	int crit;
	int toughness;
	int moveSpeed;
	int attackSpeed;
	int aggressivity;
	std::vector<int> skills;
};

class RoleAttributeTemplate {
public:
	typedef boost::shared_ptr<RoleAttributeTemplate> ptr;

	bool init();

	RoleAttribute *get(short level);

private:
	std::map<int, RoleAttribute*> m_attributes;
};
}

#endif
