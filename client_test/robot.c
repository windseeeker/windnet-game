#if !defined __linux__
#error "this program can only run under linux"
#endif

#include <stdio.h>
#include <stdlib.h>

#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>

int main(int argc, char *argv) {
    int efd = epoll_create(1024);
    if (efd == -1) {
        perror("create efd error");
        exit(1);
    }

    return 0;
}
