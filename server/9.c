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
	#include <unistd.h>
	#include <errno.h>
	#include <arpa/inet.h>
	#include <unistd.h>
	#define closesocket close
	typedef int SOCKET;
#else
#define WINSOCK_DEPRECATED_NO_WARNINGS
#include<WinSock2.h>
#include<stdint.h>
#include<wsipv6ok.h>
typedef int socklen_t;
#endif

#define SERVER_PORT 1234

void server_init(SOCKET *s)
{
    struct sockaddr_in server;
#ifdef WIN32
    struct WSAData wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) < 0) {
        printf("Error initializing the Windows Sockets Library");
        exit(1);
    }
#endif
    *s = socket(AF_INET, SOCK_STREAM, 0);
    if (*s < 0) {
        perror("Error on server socket creation\n");
        exit(1);
    }
    memset(&server, 0, sizeof(server));
    server.sin_port = htons(SERVER_PORT);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    if (bind(*s, (struct sockaddr*) &server, sizeof(server)) < 0) {
        perror("Bind error:");
        exit(1);
    }
    listen(*s, 5);
}

int connect_to_client(SOCKET *s, struct sockaddr_in *client)
{
    memset(client, 0, sizeof(struct sockaddr_in));
    int l = sizeof(struct sockaddr_in);
    int c = accept(*s, (struct sockaddr*) client, (socklen_t*)&l);
    int err = errno;
#ifdef WIN32
    err = WSAGetLastError();
#endif
    if (c < 0) {
        printf("Accept error: %d", err);
        exit(1);
    }
    return c;
}

void sendShort(SOCKET c, int16_t x)
{
    x = htons(x);
    int sizeof_x = sizeof(x);
    if (send(c, (char*)&x, sizeof(x), 0) < sizeof_x) {
        perror("Failed to send a short value");
        exit(1);
    }
    printf("Sent a short value: %hd\n", ntohs(x));
}

int16_t recvShort(SOCKET c)
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

void server_finish()
{
#ifdef WIN32
    WSACleanup();
#endif
}

int main() {
    SOCKET s;
    server_init(&s);
    while (1) {
        printf("Listening for incomming connections\n");
        struct sockaddr_in client;
        SOCKET c = connect_to_client(&s, &client);
        if (c == -1)
            continue;
        printf("Incomming connected client from: %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

        int16_t len_1 = recvShort(c);
        int16_t l1[len_1];
        for(int i=0;i<len_1;i++){
            int16_t x = recvShort(c);
            l1[i] = x;
        }

        int16_t len_2 = recvShort(c);
        int16_t l2[len_2];
        for(int i=0;i<len_2;i++){
            int16_t x = recvShort(c);
            l2[i] = x;
        }

        int16_t l3[len_2+len_1];
        int k=0;
        for(int i=0;i<len_1;i++)
        {
            int ok = 1;
            for(int j=0;j<len_2;j++)
            {
                if(l1[i]==l2[j])
                {
                    ok = 0;
                    break;
                }
            }
            if(ok==1){
                l3[k] = l1[i];
                k++;
            }
        }

        sendShort(c, k);
        for(int i=0;i<k;i++){
            sendShort(c, l3[i]);
        }

        closesocket(c);
    }
    server_finish();
}
