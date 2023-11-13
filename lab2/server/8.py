import struct
import socket
from multiprocessing import Process
import multiprocessing
def tcp_recv_uint(sock):
	try:
		x = struct.unpack("!H", sock.recv(4))[0]
		print("Received {0}".format(x))
	except struct.error as e:
		print(e)
		exit(-1)
	return x

def tcp_send_uint(sock, x):
	try:
		sock.send(struct.pack("!H", x))
		print("Sending {0}".format(x))
	except struct.error as e:
		print(e)
		exit(-1)


def tcp_server_init(ip_addr, port):
	try:
		return socket.create_server((ip_addr, port), family=socket.AF_INET, backlog=1)
	except socket.error as e:
		print(e)
		exit(-1)

def worker(client_socket):
	try:
		len_l1 = tcp_recv_uint(client_socket)
		l1 = []
		for _ in range(len_l1):
			l1.append(tcp_recv_uint(client_socket))
		len_l2 = tcp_recv_uint(client_socket)
		l2 = []
		for _ in range(len_l2):
			l2.append(tcp_recv_uint(client_socket))
		l3 = []
		for x in l1:
			if x in l2:
				l3.append(x)
		tcp_send_uint(client_socket, len(l3))
		for x in l3:
			tcp_send_uint(client_socket, x)
		print("Served the client successfully")
		client_socket.close()
	except socket.error as e:
		print(e)
		exit(-1)



if __name__ == '__main__':
	try:
		server_socket = tcp_server_init('', 1234)
		processes = []
		while True:
			print("Listening for incoming connections")
			client_socket, client_addr = server_socket.accept()
			print(f"Received incoming connection from {client_addr}")
			proc = Process(target=worker,args=(client_socket,))
			processes.append(proc)
			proc.start()
	except socket.error or multiprocessing.ProcessError or KeyboardInterrupt as e:
		print(e)
		exit(1)
	finally:
		for proc in processes:
			proc.join()
