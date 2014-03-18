#ifndef __GAME_TEMPLATE_MANAGER_H__
#define __GAME_TEMPLATE_MANAGER_H__

#include <boost/shared_ptr.hpp>

namespace Dataset {
	class RoleAttributeTemplate;
	class TaskTemplateManager;
	class SkillTemplateManager;
	class SceneTemplateManager;
	class ItemTemplateManager;
	class SoulItemTemplate;
}

class GameTemplateManager {
public:
	typedef boost::shared_ptr<GameTemplateManager> ptr;

	bool init();

	Dataset::RoleAttributeTemplate *getRoleAttriTemplate() { return m_roleAttrTemplate; }
	Dataset::TaskTemplateManager *getTaskTemplateManager() { return m_taskTempManager;  }
	Dataset::SceneTemplateManager *getSceneTemplateManager() { return m_sceneTemplateManager; }
	Dataset::SkillTemplateManager *getSkillTemplateManager() { return m_skillTempManager; }
	Dataset::ItemTemplateManager *getItemTemplateManager() { return m_itemTemplateManager; }
	Dataset::SoulItemTemplate *getSoulItemTemplate() { return m_soulItemTemplate; }

private:
	Dataset::RoleAttributeTemplate *m_roleAttrTemplate;
	Dataset::TaskTemplateManager *m_taskTempManager;
	Dataset::SceneTemplateManager *m_sceneTemplateManager;
	Dataset::SkillTemplateManager *m_skillTempManager;
	Dataset::SoulItemTemplate *m_soulItemTemplate;
	Dataset::ItemTemplateManager * m_itemTemplateManager;
};


#endif
