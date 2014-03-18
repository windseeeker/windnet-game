#ifndef __PROTO_BUF_H__
#define __PROTO_BUF_H__

struct protobuf {
    int read;
    int write;
    char data[2048 * 2];
} protobuf;

char pb_read_byte(struct protobuf* buf);
short pb_read_short(struct protobuf* buf);
int pb_read_int32(struct protobuf* buf);
long long pb_read_int64(struct protobuf* buf);
char pb_read_string(struct protobuf* buf);

int pb_read_string2(struct protobuf *buf, char *recvbuf);

int pb_write_byte();


#endif
