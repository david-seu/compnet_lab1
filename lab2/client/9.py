import struct
import socket
from multiprocessing import Process
import multiprocessing
import ctypes as ct

def tcp_send_int(sock, x):
	print("Sending {0}".format(x))
	sock.send(struct.pack("!H", x))

def tcp_recv_int(sock):
	x = struct.unpack("!H", sock.recv(2))[0]
	print("Received {0}".format(x))
	return x


def tcp_client_init(ip_addr, port):
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.connect((ip_addr, port))
	return s

if __name__ == "__main__":
    try:
        client_socket = tcp_client_init('10.152.1.29',1234)
        l1 = [int(x) for x in input("Enter the first list: ").split()]
        l2 = [int(x) for x in input("Enter the second list: ").split()]
        tcp_send_int(client_socket,len(l1))
        for x in l1:
            tcp_send_int(client_socket,x)
        tcp_send_int(client_socket,len(l2))
        for x in l2:
            tcp_send_int(client_socket,x)
        len_l3 = tcp_recv_int(client_socket)
        l3 = []
        for _ in range(len_l3):
            l3.append(tcp_recv_int(client_socket))
        print("The result is: ",l3)
    except socket.error as e:
        print(e)
        exit(1)