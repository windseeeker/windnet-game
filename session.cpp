#include "session.h"

#include <stdio.h>

#include "ring_queue.h"

	/*
int _read() {
    char buf[2048];
    int ret;
    if ((ret = ::read(_id, buf, 8)) == -1) {
        perror("socket read error\n");
        return false;
    } else if (ret == 0) {
        std::cout << "Peer closed the connection\n";
        return false;
    }
    _read_buf.append(buf, ret);
    return process(_read_buf);
}

bool Session::process(Buffer &buf) {
    int size;
    PacketHead head;
    size = _read_buf.read((char*)&head, sizeof(head));
    if (size < sizeof(head)) {
        return false;
    }
    size = _read_buf.read_buffer_sz();
    if (size < head.size) {
        return false;
    }
    DummyPacket packet;
    size = _read_buf.read((char*)&packet, head.size);
    if (size < head.size) {
        return false;
    }
    Message *msg = new Message();
    //msg->fill(packet);
    _global_queue->push_back(msg);
    return true;
}\
	*/
