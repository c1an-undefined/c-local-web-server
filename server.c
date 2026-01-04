#include <stdio.h>
#include <sys/types.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

void main() {
    //initialise Winsock2
    WSADATA wsadata;
    SOCKET server_socket;
    struct sockaddr_in server_addr;
    WSAStartup(MAKEWORD(2, 2), &wsadata);

    //create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    //bind socket
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; 
    server_addr.sin_port = htons(8080);

    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

    //listen

    listen(server_socket, 5);
    printf("Server running on http://localhost:8080\n");

    const char *response =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/plain\r\n"
    "Content-Length: 13\r\n"
    "\r\n"
    "Hello, world!";

    //accept client
    while (1) {
        SOCKET client_socket = accept(server_socket, NULL, NULL);

        char buffer[4096];
        int bytes = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        buffer[bytes] = '\0';

        printf("%s\n", buffer);

        send(client_socket, response, strlen(response), 0);
        closesocket(client_socket);
    }

    closesocket(server_socket);
    WSACleanup();

    return;
}
