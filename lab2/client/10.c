//
// Created by seu21 on 11.11.2023.
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
    if(send(c,S,sizeof(S)-1,0)<sizeof(S)-1)
    {
        perror("Failed to send a string value");
        exit(1);
    }
    printf("Sent a string value: %s\n",S);
}

void recvString(int c, char *S)
{
    if(recv(c,S,sizeof(S),0)<0)
    {
        perror("Failed to receive a string value");
        exit(1);
    }
    printf("Received a string value: %s\n",S);
}

void sendUShort(int c, uint16_t x){
    int sizeof_x = sizeof(x);
    x = htons(x);
    if (send(c, (char*)&x, sizeof_x, 0) < 0) {
        perror("Failed to send a short value");
        exit(1);
    }
    printf("Sent a short value: %hu\n", ntohs(x));
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
    char s1[1024];
    char s2[1024];
    printf("Enter a string:");
    scanf("%s",s1);
    uint16_t len_s1 = strlen(s1);
    sendUShort(c,len_s1);
    sendString(c,s1);
    printf("Enter a string:");
    scanf("%s",s2);
    uint16_t len_s2 = strlen(s2);
    sendUShort(c,len_s2);
    sendString(c,s2);
    char char_[1];
    recvString(c,char_);
    printf("The charachter with the most occurences is: %c\n",char_[0]);
    int counter = recvUShort(c);
    printf("The number of occurences is: %d\n",counter);
    connection_finish(c);
}