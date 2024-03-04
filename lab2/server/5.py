import struct
import socket
from multiprocessing import Process
import multiprocessing
import os
def tcp_server_init(ip_addr, port):
    return socket.create_server((ip_addr, port), family=socket.AF_INET, backlog=1)

def tcp_recv_string(sock):
    l = tcp_recv_uint(sock)
    string = sock.recv(l).decode()
    print("Received {0}".format(string))
    return string

def tcp_send_string(sock, string):
    tcp_send_uint(sock, len(string))
    print("Sending {0}".format(string))
    sock.send(string.encode())

def tcp_send_uint(sock, x):
    print("Sending {0}".format(x))
    sock.send(struct.pack("!H", x))

def tcp_recv_uint(sock):
    x = struct.unpack("!H", sock.recv(2))[0]
    print("Received {0}".format(x))
    return x



def worker(client_socket):
    try:
        string = tcp_recv_string(client_socket)
        ip = socket.gethostbyname(string)
        http_client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        http_client.connect((ip, 80))
        tcp_send_string(http_client, "GET/HTTP/1.0\n\n")
        string = tcp_recv_string(http_client)
        tcp_send_string(client_socket, string)
        print("Served the client successfully")
        http_client.close()
        client_socket.close()
    except socket.error as e:
        print(e)
        exit(-1)


if __name__ == "__main__":
    try:
        server_socket = tcp_server_init('', 1234)

        while True:
            print("Listening for incoming connections")
            client_socket, client_addr = server_socket.accept()
            print(f"Received incoming connection from {client_addr}")
            proc = Process(target=worker,args=(client_socket,))
            proc.start()

    except socket.error as e:
        print(e)
        exit(-1)