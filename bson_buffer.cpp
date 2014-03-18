#include "bson_buffer.h"

#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <arpa/inet.h>

#include <google/protobuf/text_format.h>

#include "protocol/packet.h"
#include "bson/CBsonObject.h"

#include "bson/bson_parser.h"

#include "peer/peer.pb.h"

const static int defaultVersion = 1256;
const char *verifyKey = "xxmp";

namespace Windnet {
	/*
void BsonBuffer::serializeToBuffer(CBsonObject &obj, Buffer &buf) {
	Packet head;
	int headLen = head.sz();
	unsigned int size ;
	unsigned char *buff = (unsigned char *)CBson::save(obj.m_Obj, (unsigned int*)&size);

	memcpy((void*)&(head.sync), (const void*)verifyKey, 4);
	head.version = htons(defaultVersion);
	head.size = htonl(size);

	int srand = head.sync % defaultVersion * size;
	int newSize = size + headLen;

	unsigned char *ptr = (unsigned char*)buf.buffer();
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
	buf.copyIn(0, newSize);
	free(buff);
}
	*/

void BsonBuffer::serializeToBuffer(Buffer &buf) {
	Packet head;
	int headLen = head.sz();
	unsigned int size = buf.readAvailable();

	memcpy((void*)&(head.sync), (const void*)verifyKey, 4);
	head.version = htons(defaultVersion);
	head.size = htonl(size);

	int srand = head.sync % defaultVersion * size;
	int newSize = size + headLen;

	unsigned char buffer[newSize];

	unsigned char *buff = (unsigned char*)buf.buffer();
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
	buf.reset();
	buf.copyIn((char*)buffer, newSize);
}

struct ProtoHead {
	unsigned int type;
	unsigned int size;
};

int BsonBuffer::parseFromBuffer(int id) {

#if 0   //test google protocol buffer
	ProtoHead head;
	int headLen = sizeof(ProtoHead);
	if (readAvailable() < headLen) {
		fprintf(stdout, "wait for head size\n");
		return 0;
	}
	memcpy((void*)&head, m_buffer + m_read, headLen);
	if (readAvailable()  < head.size) {
		fprintf(stdout, "wait for buffer body\n");
		return 0;
	}
	m_read += headLen;
	fprintf(stdout, "Recv google protocol buffer, type %d\n", head.type);
	size_t protoSize = head.size - headLen;

	peer::RegisterServer message;
	std::string data;
	data.append(m_buffer + m_read, protoSize);
	if (!google::protobuf::TextFormat::ParseFromString(data, &message)) {
		fprintf(stdout, "Parse google protocol buffer error\n", head.type);
		return -1;
	}
	m_read += protoSize;

	fprintf(stdout, "Message server_name %s, server_id %d , param %s\n",
			message.server_name().c_str(), message.server_id(), message.param().c_str());
	fflush(stdout);

#else

	Packet head;
	int headLen = head.sz();
	if (readAvailable() < headLen) {
		fprintf(stdout, "wait for head size\n");
		return 0;
	}
	memcpy((void*)&head, m_buffer + m_read, headLen);
	if (memcmp(&head.sync, verifyKey, 4) != 0) {
		fprintf(stdout, "Invalid head sync field\n");
		return -1;
	}
	head.size = ntohl(head.size);
	head.version = ntohs(head.version);
	if (readAvailable() - head.sz() < head.size) {
		fprintf(stdout, "wait for buffer body\n");
		return 0;
	} else if (head.size < 0 || head.size > MAX_BUFFER_SIZE) {
		fprintf(stdout, "We encounter fatal error while parse bson buffer, invalid head size\n");
		return -1;
	}

	m_read += headLen;
	int srand = head.sync % head.version * head.size;

	//unsigned char *buffer = (unsigned char*)malloc(head.size);
	unsigned char buffer[head.size];
	unsigned char *ptr = buffer;
	unsigned int count = 0;
	while (count < head.size) {
		char a = std::min((unsigned int)4, head.size - count);
		volatile int val = 0;
		memcpy((void*)(&val), (m_buffer + m_read + count), a);
		val ^= srand;
		memcpy((void*)ptr, (const void*)&val, a);
		ptr += a;
		count += a;
	}
	m_read += head.size;

	try {
		m_protocol  = BSON::parse((const char*)buffer, head.size);
	} catch (...) {
	}

	/////////////////////////////////////////
#if 0
	//Test core dump
	for (long long i = 0; i < 10; ++i) {
		BSON::Object obj = BSON::parse((const char*)buffer, head.size);
		std::string proto = BSON::getStringVal("Command", &obj);

		BSON::Object body1 = BSON::getObjectVal("Body", (BSON::Object*)&obj);
		std::string name = BSON::getStringVal("Name", &body1);
		std::cout << "Protocol " << proto << "   " << i << " name : " << name << std::endl;

		BSON::Object response, body;
		BSON::setStringVal(response, "Command", "OtherRunInLobby");
		BSON::setIntVal(body, "Id", 1);
		BSON::setDoubleVal(body, "X", 1.2);
		BSON::setDoubleVal(body, "X", 3.1);
		BSON::setObjectVal(response, "Body", body);
		Buffer buf;
		BSON::serializeToBuffer(buf, response);
		BsonBuffer::serializeToBuffer(buf);

		//::write(id, buf.buffer(), buf.readAvailable());
		std::cout << "send walking packet " << i  << std::endl;
	}
	//free(buffer);
#endif
	///////////////////////////////////////////

	return 0;

#endif
}

}
