import socket, struct

"""
! - (from or to) network
c - char
h - short
H - unsigned short
i - int
I - unsigned int
q - long long
Q - unsigned long long
f - float
d - double
"""

def tcp_send_int(sock, x):
	print("Sending {0}".format(x))
	sock.send(struct.pack("!i", int(x)))

def tcp_recv_int(sock):
	x = struct.unpack("!i", sock.recv(4))[0]
	print("Received {0}".format(x))
	return x

def tcp_client_init(ip_addr, port):
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.connect((ip_addr, port))
	return s

if __name__ == "__main__":
    try:
        client_socket = tcp_client_init('172.30.111.139',1234)
        integer = input("Enter an integer")
        tcp_send_int(client_socket,integer)
        len_div = tcp_recv_int(client_socket)
        div = []
        for _ in range(len_div):
            div.append(tcp_recv_int(client_socket))

        print(div)
    except socket.error as e:
        print(e)
        exit(1)
