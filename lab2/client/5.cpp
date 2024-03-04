#include <cstdlib>
#include <cstring>
#include <iostream>
#ifndef _WIN32
#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <unistd.h>
	#define closesocket close
	typedef int SOCKET;
#else
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<WinSock2.h>
#include<cstdint>
#include<wsipv6ok.h>
#endif

#define SERVER_PORT 1234
#define SERVER_IP "172.19.1.199"
using namespace std;

int connection_init(int &c)
{
#ifdef WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) < 0) {
        perror("Error initializing the Windows Sockets Library");
        return -1;
    }
#endif
    c = socket(AF_INET, SOCK_STREAM, 0);
    if (c < 0) {
        cerr << "Error on socket creation\n";
        exit(1);
    }
    sockaddr_in server{};
    memset(&server, 0, sizeof(server));
    server.sin_port = htons(SERVER_PORT);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    if (connect(c, (struct sockaddr*) &server, sizeof(server)) < 0) {
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


void sendUShort(int c, uint16_t x)
{
    x = htons(x);
    int sizeof_x = sizeof(x);
    if (send(c, (char*)&x, sizeof(x), 0) < sizeof_x) {
        cerr << "Failed to send a short value";
        exit(1);
    }
    uint16_t copy = ntohs(x);
    cout << "Sent a short value: " << copy << "\n";
}


void sendString(int c, char* s){
    int len = strlen(s);
    sendUShort(c, len);
    if(send(c, s, len, 0) < len)
    {
        perror("Error on sending message to server\n");
        exit(1);
    }
    cout << "Sent a string: " << s << "\n";
}


uint16_t recvUShort(int c)
{
    uint16_t x;
    int sizeof_x = sizeof(x);
    if (recv(c, (char*)&x, sizeof(x), 0) < sizeof_x) {
        cerr << "Failed to receive a short value";
        exit(1);
    }
    uint16_t copy = ntohs(x);
    cout << "Received a short value: " << copy << "\n";
    return ntohs(x);
}

void recvString(int c, char* s){
    int len = recvUShort(c);
    int bytes;
    if((bytes=recv(c, s, len, 0)) < len)
    {
        perror("Error on receiving message from server\n");
        exit(1);
    }
    cout << "Received a string: " << s << "\n";
}



int main() {
    int c;
    connection_init(c);
    char buffer[100];
    cout << "Enter a string: ";
    cin.getline(buffer, 100);
    sendString(c, buffer);
    char result[2048];
    recvString(c,result);
    cout << "Received: " << result << "\n";
    connection_finish(c);
}
