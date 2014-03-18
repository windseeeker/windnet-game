#include "scene_manager.h"

#include "buffer.h"

#include "player_session.h"
#include "scene.h"
#include "dataset/scene_template_manager.h"

using namespace Windnet;

bool SceneManager::initScenes(Dataset::SceneTemplateManager *stm) {
	std::list<Dataset::SceneTemplate*> l;
	stm->getAllScenes(l);
	std::list<Dataset::SceneTemplate*>::iterator it = l.begin();
	for (; it != l.end(); ++it) {
		Scene::ptr scene(new Scene);
		scene->m_template = *it;
		m_scenes[(*it)->sceneId] = scene;
	}
	return true;
}

Scene::ptr SceneManager::get(int sceneId) {
	std::map<int, Scene::ptr>::iterator it = m_scenes.find(sceneId);
	return it == m_scenes.end() ? Scene::ptr() : it->second;
}
