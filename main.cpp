#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/un.h>
#include <netinet/in.h>

#include <stdio.h>
#include <stdlib.h>

#include <iostream>

#include "ring_queue.h"
#include "player_session.h"
#include "socket.h"
#include "game_server.h"
#include "socket_server.h"
#include "thread.h"
#include "timer.h"
#include "log.h"

static int threadCount = 1;

int g_logLevel = LOG_INFO;

using namespace Windnet;
using namespace Windnet::Net;

void loadFromEnvironment() {
	char *env = getenv("port");
	if (env) {
		//int listen_fd = atoi(env);
	}
	env = getenv("thread_num");
	if (env) {
		threadCount = atoi(env);
    }
}

int main() {
	loadFromEnvironment();

	GameServer gameServer;
	gameServer.init();

	pthread_t threads[threadCount];
	for (int i = 0; i < threadCount; ++i) {
		pthread_create(&threads[i], 0, WorkThread::threadRoutine, &gameServer);
	}

	std::vector<SocketServer::SockThreadParam*> params;
	params.resize(3);
	params[0] = new SocketServer::SockThreadParam;
	params[0]->sockServer = gameServer.sockServer();
	params[0]->port = 20000;
	params[0]->serviceType = Socket::SERVICE_GATE;

	params[1] = new SocketServer::SockThreadParam;
	params[1]->sockServer = gameServer.sockServer();
	params[1]->port = 20001;
	params[1]->serviceType = Socket::SERVICE_SHORT;

	params[2] = new SocketServer::SockThreadParam;
	params[2]->sockServer =  gameServer.sockServer();
	params[2]->port = 20003;
	params[2]->serviceType = Socket::SERVICE_LONG;

	pthread_t acceptThreads[3];//, acceptThread12, acceptThread3; //timerThread;
	for (int i = 0; i < 3; ++i) {
		pthread_create(&acceptThreads[i], 0, SocketServer::socketAcceptRoutine, params[i]);
	}
	//pthread_create(&timerThread, 0, TimerManager::timerRoutine, 0);

	gameServer.start();
	for (int i = 0; i < 3; ++i) {
		pthread_join(acceptThreads[i], 0);
		delete params[i];
	}
	for (int i = 0; i < threadCount; ++i) {
		pthread_join(threads[i], 0);
	}

	return 0;
}
