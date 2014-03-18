#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#include <errno.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <google/protobuf/text_format.h>
#include "peer.pb.h"

static int clientFd;
static struct sockaddr_in remote;

const static int defaultVersion = 1256;

static const char *ip = "127.0.0.1";


void *recvThread(void *arg) {
	char buffer[1024];
    while (1) {
        int n = read(clientFd, buffer, sizeof(buffer));
        if (n == -1) {
            if (errno == EAGAIN) {
                write(STDOUT_FILENO, "EINTR", sizeof("EINTR"));
                continue;
            }
            continue;
        }
    }
}

static void connectServer(int arg) {
    int ret = -1;
    do {
        clientFd = socket(AF_INET, SOCK_STREAM, 0);
        remote.sin_family = AF_INET;
        remote.sin_addr.s_addr = inet_addr(ip);
        remote.sin_port = htons(20000);
        ret = connect(clientFd, (struct sockaddr*)&remote, sizeof(remote));
        perror("connect error");
        sleep(3);
    } while (ret == -1);
}

void interrupt(int val) {
    fprintf(stdout, "recv sigint\n");
    exit(0);
}

struct Packet {
	unsigned int type;
	unsigned int size;
};

int sendBuffer(const std::string &data) {
	Packet head;
	head.type = 101;
	head.size = sizeof(head) + data.size();

	int index = 0;
	char buffer[2048];
	memcpy(buffer + index, &head.type, sizeof(head.type));
	index += sizeof(head.type);
	memcpy(buffer + index, &head.size, sizeof(head.size));
	index += sizeof(head.size);

	memcpy(buffer + index, data.data(), data.size());
	index += data.size();
	if (write(clientFd, buffer, index) == -1) {
		perror("Send buffer error");
	}
	return 0;
}

int main(int argc, char **argv) {
	peer::RegisterServer message;
    message.set_server_name("test_name");
	message.set_server_id(2014);
	message.set_param("this is the tese server");
	connectServer(0);

	std::string data;
	if (!google::protobuf::TextFormat::PrintToString(message, &data)) {
		fprintf(stdout, "google::protobuf::textformat error\n");
		return 0;
	}
	fprintf(stdout, "string data %s\n", data.c_str());
	sendBuffer(data);

	fprintf(stdout, "data begins\n");
	for (size_t i = 0; i < data.size(); ++i) {
		fprintf(stdout, "%c", data[i]);
	}
	fprintf(stdout, "data ends\n");
	peer::RegisterServer recvMsg;
	if (!google::protobuf::TextFormat::ParseFromString(data, &recvMsg)) {
		fprintf(stdout, "parse string  error\n");
		return 0;
	}

	fprintf(stdout, "register server id %d\n", recvMsg.server_id());

    //struct sigaction action, oldaction;
    //action.sa_handler = reconnect;
    //action.sa_flags = 0;
    //sigaction(SIGPIPE, &action, &oldaction);
    signal(SIGPIPE, connectServer);
    signal(SIGINT, interrupt);

    pthread_t t1, t2;
    //pthread_create(&t1, 0, sendThread, 0);
    pthread_create(&t2, 0, recvThread, 0);
    //pthread_join(t1, 0);
    pthread_join(t2, 0);
    return 0;
}
