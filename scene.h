#ifndef __SCENE_H__
#define __SCENE_H__

#include <boost/shared_ptr.hpp>

#include <map>

namespace Windnet {
	class Buffer;
}

namespace Dataset {
	class SceneTemplate;
}

class PlayerSession;
class SceneManager;
class Scene {
	friend class SceneManager;
public:
	typedef boost::shared_ptr<Scene> ptr;

	void addPlayer(int id, PlayerSession *ps);
	void removePlayer(int roleId);

	void sendSurrounding(int id, PlayerSession *ps);

	void leaveScene(PlayerSession *ps);
	void broadcast(Windnet::Buffer &buffer, int selfRoleId = -1);

private:
	std::map<int, PlayerSession* > m_players;
	Dataset::SceneTemplate *m_template;
};

#endif
