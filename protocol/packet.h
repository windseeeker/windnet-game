#ifndef __PACKET_H__
#define __PACKET_H__

#pragma pack(1)

struct Packet {
	int sync;
	unsigned short version;
	unsigned int size;

	size_t sz() { return sizeof(sync) + sizeof(version) + sizeof(size); }
};
#pragma pack()

struct protobuf;
struct dummy {
	struct Packet head;
	struct protobuf *pb;
};

int handle_packet(struct dummy *p, void *ud);

#endif

