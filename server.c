#include <stdio.h>
#include <sys/types.h>
#include <WinSock2.h>

void main() {
    //initialise Winsock2
    WSADATA wsadata;
    WSAStartup(MAKEWORD(2, 2), &wsadata);

    // int socket(int af, int type, int protocol);

    return 0;
}
