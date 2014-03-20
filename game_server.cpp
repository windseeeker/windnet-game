#include "game_server.h"

#include <stdlib.h>

#include "log.h"
#include "server_resource.h"
#include "socket_server.h"
#include "peer/server_manager.h"

using namespace Windnet;
using namespace Windnet::Net;

GameServer::GameServer() {
	m_resource = new ServerResource;
	m_sockServer = new SocketServer(m_resource);
	//m_serverManager.reset(new ServerManager);
}

GameServer::~GameServer() {
}

void GameServer::init() {
	srand(time(NULL));

	m_sockServer->initServer();
	m_resource->init();
}

void GameServer::start() {
	m_sockServer->startServer();
}
