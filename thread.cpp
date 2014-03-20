#include "thread.h"

#include <stdio.h>
#include <unistd.h>

#include "message.h"
#include "ring_queue.h"
#include "servlet.h"
#include "game_server.h"
#include "server_resource.h"

namespace Windnet {
void * WorkThread::threadRoutine(void *arg) {
	//fprintf(stdout, "Start working routine\n");
	GameServer *gameServer = (GameServer*)arg;
	ServerResource *res = gameServer->serverRes();

	for (; ;) {
		Message *msg = res->getQueue()->pop();
		//Windnet::Message *msg = gameServer->serverQueue()->pop();
		if (!msg) {
			usleep(1000); //10 macro
			continue;
		}
		fprintf(stdout, "handle message\n");
		res->getServletsDispatch()->dispatchMessage(res, msg);
		//delete msg;
	}
	return 0;
}

}
