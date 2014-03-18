#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

#include <boost/shared_ptr.hpp>

#include <map>

namespace Dataset {
	class SceneTemplateManager;
}

class Scene;
class SceneManager {
public:
	typedef boost::shared_ptr<SceneManager> ptr;

	bool initScenes(Dataset::SceneTemplateManager *stm);

	boost::shared_ptr<Scene> get(int sceneId);

private:
	std::map<int, boost::shared_ptr<Scene> > m_scenes;
};

#endif
