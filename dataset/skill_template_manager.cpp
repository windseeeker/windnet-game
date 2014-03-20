#include "skill_template_manager.h"

#include "../assert.h"

#include "csv_reader.h"

namespace Dataset {
bool SkillTemplateManager::init() {
	CSVReader reader;
	if (!reader.loadFile("./data/skill.csv")) {
		ASSERT(false);
	}
	//int val;
    do {
		SkillTemplate *skill = new SkillTemplate;
		ASSERT(reader.Attribute("Task_id", skill->skillId));
		ASSERT(reader.Attribute("Task_Type", skill->skillType));

		m_skills.insert(std::make_pair(skill->skillId, skill)); 
    }  while (reader.moveNext());

	return true;
}

SkillTemplate *SkillTemplateManager::get(int id) {
	std::map<int, SkillTemplate*>::iterator it = m_skills.find(id);
	return it == m_skills.end() ? NULL : it->second;
}
}
