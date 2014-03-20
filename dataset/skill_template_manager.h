#ifndef __SKILL_TEMPLATE_MANAGER_H__
#define __SKILL_TEMPLATE_MANAGER_H__

#include <boost/shared_ptr.hpp>

#include <map>
#include <vector>

namespace Dataset {
struct SkillTemplate {
	typedef boost::shared_ptr<SkillTemplate> ptr;

	enum {
	};
	int skillType;
	int skillId;
};

class SkillTemplateManager {
public:
	typedef boost::shared_ptr<SkillTemplateManager> ptr;

	bool init();

	SkillTemplate *get(int id);

private:
	std::map<int, SkillTemplate*> m_skills;
};
}

#endif
