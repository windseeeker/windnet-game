#include "game_template_manager.h"

#include "dataset/role_attribute_template.h"
#include "dataset/item_template_manager.h"
#include "dataset/task_template_manager.h"
#include "dataset/scene_template_manager.h"
#include "dataset/skill_template_manager.h"
#include "dataset/soul_item_template.h"

bool GameTemplateManager::init() {
	m_roleAttrTemplate = new Dataset::RoleAttributeTemplate;
	m_roleAttrTemplate->init();

	m_taskTempManager = new Dataset::TaskTemplateManager;
	//m_taskTempManager->init();

	m_skillTempManager = new Dataset::SkillTemplateManager;

	m_sceneTemplateManager = new Dataset::SceneTemplateManager;
	m_sceneTemplateManager->init();

	m_itemTemplateManager = new Dataset::ItemTemplateManager;
	m_itemTemplateManager->init();

	m_soulItemTemplate = new Dataset::SoulItemTemplate;
	m_soulItemTemplate->init();

	return true;
}

