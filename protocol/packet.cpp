#include "packet.h"

#include <stdio.h>

typedef int (*handler)(struct dummy*, void* ud);

static handler handle_func[3000];

namespace Windnet {
int packet_dispatch(struct dummy *packet, void * ud);
int login_request(struct dummy *packet, void *ud);

int register_cb(int type, handler func) {
    type = type / 100 + 1;
    handle_func[type] = func;
    return 0;
}

int packet_dispatch(struct dummy *packet, void *ud) {
	int type = packet->head.type;
	if (type < 0 || type > 3000) {
		fprintf(stderr, "no such packet type: %d\n", type);
		return -1;
	}
	return (*handle_func[type])(packet, ud);
}

void init_game_module() {
    int i = 1;
    for (; i <= 99; ++i) {
        //register_cb(1, login_request);
    }
}

}
