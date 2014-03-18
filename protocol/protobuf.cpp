#include <string.h>

struct protobuf {
	int read;
	int write;
	char data[2048 * 2];
} protobuf;

char pb_read_byte(struct protobuf* buf) {
	char *p = buf->data + buf->read;
	buf->read += sizeof(char);
	return *p;
}

short pb_read_short(struct protobuf* buf) {
    short *p = (short*)(buf->data + buf->read);
    buf->read += sizeof(short);
    return *p;
}

int pb_read_int32(struct protobuf* buf) {
    int *p = (int*)(buf->data + buf->read);
    buf->read += sizeof(int);
    return *p;
}

long long pb_read_int64(struct protobuf* buf) {
    long long *p = (long long*)(buf->data + buf->read);
    buf->read += sizeof(long long);
    return *p;
}

int pb_read_string(struct protobuf* buf, char *recvbuf, int len) {
    char *p = buf->data + buf->read;
    memcpy(recvbuf, p, len);
    buf->read += len;
    return 0;
}

int pb_read_string2(struct protobuf *buf, char *recvbuf) {
    int string_len = pb_read_short(buf);
    return pb_read_string(buf, recvbuf, string_len);
}

int pb_write_byte(struct protobuf *buf, const char byte) {
    memcpy(buf->data + buf->write, &byte, sizeof(char));
    buf->write += sizeof(char);
    return 0;
}

int pb_write_short(struct protobuf *buf, const short val) {
    memcpy(buf->data + buf->write, &val, sizeof(short));
    buf->write += sizeof(short);
    return 0;
}

int pb_write_int32(struct protobuf *buf, const int val) {
    memcpy(buf->data + buf->write, &val, sizeof(int));
    buf->write += sizeof(int);
    return 0;
}

int pb_write_int64(struct protobuf *buf, const long long val) {
    memcpy(buf->data + buf->write, &val, sizeof(long long));
    buf->write += sizeof(long long);
    return 0;
}
