#include "scene_template_manager.h"

#include "../assert.h"

#include "csv_reader.h"

namespace Dataset {
bool SceneTemplateManager::init() {
	CSVReader reader;
	if (!reader.loadFile("./data/mapcfg.csv")) {
		ASSERT(false);
	}
	int val;
    do {
		SceneTemplate *scene = new SceneTemplate;
		ASSERT(reader.Attribute("Map_id", scene->sceneId));
		ASSERT(reader.Attribute("Map_type", val));
		scene->sceneType = val;

		m_scenes.insert(std::make_pair(scene->sceneId, scene));
    }  while (reader.moveNext());

	return true;
}

void SceneTemplateManager::getAllScenes(std::list<SceneTemplate*> &l) {
	std::map<int, SceneTemplate*>::iterator it = m_scenes.begin();
	for (; it != m_scenes.end(); ++it) {
		l.push_back(it->second);
	}
}

SceneTemplate *SceneTemplateManager::get(int id) {
	std::map<int, SceneTemplate*>::iterator it = m_scenes.find(id);
	return it == m_scenes.end() ? NULL : it->second;
}
}
