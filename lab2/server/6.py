import random
import struct
import socket
from multiprocessing import Process
import multiprocessing
import os
import signal

def tcp_server_init(ip_addr, port):
    return socket.create_server((ip_addr, port), family=socket.AF_INET, backlog=1)


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



def worker(client_socket,rnd_number_server):
    try:
        nr_tries = 0
        while True:
            nr_tries += 1
            rnd_number_client = tcp_recv_uint(client_socket)
            if rnd_number_client == rnd_number_server:
                tcp_send_string(client_socket,f"You win - within {nr_tries} tries!")
                break
            elif rnd_number_client < rnd_number_server:
                tcp_send_string(client_socket,"Larger")
            else:
                tcp_send_string(client_socket,"Smaller")
        client_socket.close()
        exit(0)
    except socket.error as e:
        print(e)
        exit(-1)


if __name__ == "__main__":
    try:
        server_socket = tcp_server_init('', 1234)
        while True:
            rnd_number = random.randint(0, 200)
            print("Listening for incoming connections")
            client_socket, client_addr = server_socket.accept()
            print(f"Received incoming connection from {client_addr}")
            proc = Process(target=worker,args=(client_socket,rnd_number))
            proc.start()

    except socket.error as e:
        print(e)
        exit(-1)