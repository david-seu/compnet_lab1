import struct
import socket
from multiprocessing import Process
import multiprocessing
def tcp_recv_int(sock):
	x = struct.unpack("!i", sock.recv(4))[0]
	print("Received {0}".format(x))
	return x

def tcp_send_string(sock, string):
	print("Sending {0}".format(string))
	sock.send(string.encode('ascii'))

def tcp_recv_string(sock):
	string = sock.recv(1024).decode('ascii')
	print("Received {0}".format(string))
	return string


def tcp_server_init(ip_addr, port):
	return socket.create_server((ip_addr, port), family=socket.AF_INET, backlog=1)


def worker(client_socket):
    try:
        string = tcp_recv_string(client_socket)
        i = tcp_recv_int(client_socket)
        l = tcp_recv_int(client_socket)
        tcp_send_string(client_socket, string[i:i+l])
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


