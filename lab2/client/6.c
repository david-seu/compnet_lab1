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
#define SERVER_IP "10.152.1.29"

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

void sendString(int c, char *S)
{
    if(send(c,S,sizeof(S),0)<0)
    {
        perror("Failed to send a string value");
        exit(1);
    }
    printf("Sent a string value: %s\n",S);
}


void sendChar(SOCKET c, char q)
{
    int sizeof_x = sizeof(q);
    send(c, (char*)&q, sizeof(q), 0);
    printf("Sent a char value: %c\n", q);
}

uint16_t recvUShort(int c)
{
    uint16_t x;
    int sizeof_x = sizeof(x);
    if (recv(c, (char*)&x, sizeof(x), 0) < sizeof_x) {
        perror("Failed to receive a short value");
        exit(1);
    }
    printf("Received a short value: %hu\n", ntohs(x));
    return ntohs(x);
}


int main(){
    int c;
    connection_init(&c);
    char S[1024];
    char char_to_send[1];
    printf("Enter a char:");
    scanf("%s",char_to_send);
    sendString(c,char_to_send);
    printf("Enter a string:");
    scanf("%s",S);
    sendString(c,S);
    int len_pos = recvUShort(c);
    int pos[len_pos];
    for(int i=0;i<len_pos;i++)
    {
        pos[i]=recvUShort(c);
        printf("%d ",pos[i]);
    }
    connection_finish(c);
}