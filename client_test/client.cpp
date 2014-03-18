#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#if defined LINUX
#include <sys/epoll.h>
#endif

#include <errno.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "protocol.h"


#include "../bson/CBsonObject.h"
#include "../bson/bson_parser.h"

#include "../bson_buffer.h"
#include "CProtocol.h"

static int clientFd;
static struct sockaddr_in remote;

const static short defaultVersion = 1256;

static const char *ip = "127.0.0.1";

using namespace Windnet;

int sendBuffer(int fd, const char *buffer, int bufLen) {
	if (write(fd, buffer, bufLen) == -1) {
		perror("Send buffer error");
	}
	return 0;
}

int login(int fd) {
	BSON::Object obj, body;
	BSON::setStringVal(obj, "Command", "Register");

	BSON::setStringVal(body, "Name", "ClientTest");
	BSON::setStringVal(body, "Pwd", "testpasswd");
	BSON::setObjectVal(obj, "Body", body);
	Buffer buf;
	BSON::serializeToBuffer(buf, obj);
	BsonBuffer::serializeToBuffer(buf);
    //ps->sendData(buf);
	write(clientFd, buf.buffer(), buf.readAvailable());
	return 0;
}

int loadServers() {
	BSON::Object obj;
	BSON::setStringVal(obj, "Command", "LoadServers");

    Buffer buf;
	BSON::serializeToBuffer(buf, obj);
	BsonBuffer::serializeToBuffer(buf);
    //ps->sendData(buf);
    write(clientFd, buf.buffer(), buf.readAvailable());
	return 0;
}

void doWalk() {
	BSON::Object response, body;
	double posX = 101.2;
	double posY = 201.1;

	BSON::setStringVal(response, "Command", "RunInLobby");
	BSON::setDoubleVal(body, "X", posX);
	BSON::setDoubleVal(body, "Y", posY);
	BSON::setObjectVal(response, "Body", body);
	Buffer buf;
	BSON::serializeToBuffer(buf, response);
	BsonBuffer::serializeToBuffer(buf);
	write(clientFd, buf.buffer(), buf.readAvailable());
}

#pragma pack(1)
struct Packet {
	int sync;
	unsigned short version;
	unsigned int size;

	size_t sz() { return sizeof(sync) + sizeof(version) + sizeof(size); }
};
#pragma pack()

void serializeToBuffer(Windnet::Buffer &buffer) {

}

void sendData(CBsonObject &bson);

//Test bson protobuf serialize
void Register() {
	CBsonObject bson;
    bson.createNew();
    bson.setStringValue("Command", "Register");
    bson.setStringValue("Name", "ClientTest");
    bson.setStringValue("Pwd", "testpasswd");
    //bson.setStringValue("DeviceMode", "moooooooooooode");
    //bson.setStringValue("Type", "system");
    bson.setStringValue("DeviceId", "id1001");
	//bson.setIntValue("ServerId", 101);

	return sendData(bson);
}

void sendData(CBsonObject &bson) {
	Packet head;
	int headLen = head.sz();
	int size;
	unsigned char *buff = (unsigned char *)CBson::save(bson.m_Obj, (unsigned int*)&size);

    memcpy((void*)&(head.sync), (const void*)"xxmp", 4);
    head.version = htons(defaultVersion);
    head.size = htonl(size);
    int srand = head.sync % head.version * size;
    int totalLen = size + headLen;

    unsigned char buffer[totalLen];
    unsigned char *ptr = buffer;
    memcpy(ptr, &head, headLen);
    ptr += headLen;

    unsigned int count = 0;
    while (count < size) {
        char a = std::min((unsigned int)4, size - count);
        volatile int val = 0;
        memcpy((void*)&val, (buff + count), a);
        val ^= srand;
        memcpy((void*)ptr, (const void*)&val, a);
        ptr += a;
        count += a;
    }
    free(buff);          //////////////need to be optimized
    write(clientFd, buffer, totalLen);
}


void createRole() {
	CBsonObject bson;
    bson.createNew();
	bson.setStringValue("Command", "CreateRole");
	bson.setStringValue("Name", "role1");
	bson.setStringValue("Token", "9bce46d18b3742dbb50935994458da4f");
	bson.setIntValue("ProfessId", 1);
	return sendData(bson);
}

void *sendThread(void *arg) {
	long long i = 0;
    while (1) {
		loadServers();
		//login(clientFd);
		std::cout << "Sending " << i++ << std::endl;
		//serializeToBuffer();
		//usleep(10000);
	}
}

void *recvThread(void *arg) {
	fprintf(stdout, "Begin recv data\n");
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
		fprintf(stdout, "Recv %d bytes data \n", n);
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

    //int verify = 49;
    //write(clientFd, &verify, sizeof(verify));
}

void interrupt(int val) {
    fprintf(stdout, "recv sigint\n");
    exit(0);
}

int main(int argc, char **argv) {
	connectServer(0);
	//createRole();
	//serializeToBuffer();
	//login(clientFd);
	loadServers();
	doWalk();

    //struct sigaction action, oldaction;
    //action.sa_handler = reconnect;
    //action.sa_flags = 0;
    //sigaction(SIGPIPE, &action, &oldaction);
    signal(SIGPIPE, connectServer);
    signal(SIGINT, interrupt);

    pthread_t t1, t2;
    pthread_create(&t1, 0, sendThread, 0);
    //pthread_create(&t2, 0, recvThread, 0);
    pthread_join(t1, 0);
    pthread_join(t2, 0);
    return 0;
}
