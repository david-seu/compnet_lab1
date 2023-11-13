//
// Created by seu21 on 04.11.2023.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifndef _WIN32
#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <netinet/ip.h>
	#include <arpa/inet.h>
	#include <unistd.h>
	#define closesocket close
	typedef int SOCKET;
#else
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<WinSock2.h>
#include<wsipv6ok.h>
#include <stdint-gcc.h>

#endif

#define SERVER_PORT 1234
#define SERVER_IP "10.152.0.99"

int connection_init(int *c)
{
#ifdef WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) < 0) {
        perror("Error initializing the Windows Sockets Library");
        return -1;
    }
#endif
    *c = socket(AF_INET, SOCK_STREAM, 0);
    if (*c < 0) {
        perror("Error on socket creation\n");
        exit(1);
    }
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_port = htons(SERVER_PORT);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    if (connect(*c, (struct sockaddr*) &server, sizeof(server)) < 0) {
        perror("Error on server connection\n");
        exit(1);
    }
}

void connection_finish(int c)
{
#ifdef WIN32
    WSACleanup();
    closesocket(c);
#else
    close(c);
#endif
}


char recvChar(int c)
{
    char x[1];
    int sizeof_x = sizeof(x);
    if (recv(c, x, 1, 0) < sizeof_x) {
        perror("Failed to receive a char value");
        exit(1);
    }
    printf("Received a char value: %c\n", x[0]);
    return x[0];
}
int16_t recvShort(int c)
{
    int16_t x;
    int sizeof_x = sizeof(x);
    if (recv(c, (char*)&x, sizeof(x), 0) < sizeof_x) {
        perror("Failed to receive a short value");
        exit(1);
    }
    printf("Received a short value: %hd\n", ntohs(x));
    return ntohs(x);
}

void sendShort(int c, int16_t x)
{
    x = htons(x);
    int sizeof_x = sizeof(x);
    if (send(c, (char*)&x, sizeof(x), 0) < sizeof_x) {
        perror("Failed to send a short value");
        exit(1);
    }
    printf("Sent a short value: %hu\n", ntohs(x));
}

void sendString(int c, char *S)
{
    int n = strlen(S);
    int16_t n1 = n;
    sendShort(c, n1);
    send(c, S, n, 0);
}

int main() {
    int c;
    connection_init(&c);
    char s1[100];
    printf("Enter a string: ");
    gets(s1);
    char s2[100];
    printf("Enter a string: ");
    gets(s2);

    sendString(c, s1);
    sendString(c, s2);

    int16_t x = recvShort(c);
    char q = recvChar(c);

    printf("Char %c apperead %d times\n", q, x);
    connection_finish(c);
}
