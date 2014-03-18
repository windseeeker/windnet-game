#ifndef __SCENE_TEMPLATE_MANAGER_H__
#define __SCENE_TEMPLATE_MANAGER_H__

//#include <boost/shared_ptr.hpp>

#include <list>
#include <map>

namespace Dataset {
struct SceneTemplate {

	enum {
		SCENE_TYPE_TOWN = 1,
		SCENE_TYPE_SINGLE_INSTANCE = 2,
		SCENE_TYPE_MULTI_INSTANCE = 3,
		SCENE_TYPE_PVP = 4
	};
	short sceneType;
	int sceneId;

};

class SceneTemplateManager {
public:
	bool init();

	void getAllScenes(std::list<SceneTemplate*> &l);
	SceneTemplate *get(int id);

private:
	std::map<int, SceneTemplate*> m_scenes;
};
}

#endif
