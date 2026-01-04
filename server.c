#include <stdio.h>
#include <sys/types.h>
#include <WinSock2.h>
#include <stdlib.h>
#include <string.h>

#pragma comment(lib, "ws2_32.lib")

int main(void) {
    //initialise Winsock2
    WSADATA wsadata;
    SOCKET server_socket;
    struct sockaddr_in server_addr;
    WSAStartup(MAKEWORD(2, 2), &wsadata);

    //create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket == INVALID_SOCKET) {
        printf("Socket creation failed\n");
        return 1;
    }

    //bind socket
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; 
    server_addr.sin_port = htons(8080);

    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

    //listen
    listen(server_socket, 5);
    printf("Server running on http://localhost:8080\n");

    //accept client
    while (1) {
        SOCKET client_socket = accept(server_socket, NULL, NULL);

        char buffer[4096];
        int bytes = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes <= 0) {
            closesocket(client_socket);
            continue;
        }
        buffer[bytes] = '\0';

        printf("%s\n", buffer);

        //load html file
        FILE *file = fopen("test.html", "rb");
        if (!file) {
            const char *not_found =
                "HTTP/1.1 404 Not Found\r\n"
                "Content-Type: text/plain\r\n"
                "Content-Length: 9\r\n"
                "\r\n"
                "Not Found";

            send(client_socket, not_found, strlen(not_found), 0);
            closesocket(client_socket);
            continue;
        } 
        
        //get file size
        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        rewind(file);

        if (file_size <= 0) {
            fclose(file);
            closesocket(client_socket);
            continue;
        } 

        //read file to file_buffer
        char *file_buffer = malloc(file_size);
        fread(file_buffer, 1, file_size, file);
        fclose(file);

        //headers
        char header[256];
        int header_length = snprintf(header, sizeof(header),
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: %ld\r\n"
            "\r\n",
            file_size
        );

        send(client_socket, header, header_length, 0);
        send(client_socket, file_buffer, file_size, 0);
        free(file_buffer);
        
        closesocket(client_socket);
    }

    closesocket(server_socket);
    WSACleanup();

    return 1;
}
