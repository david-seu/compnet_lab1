import socket
import struct


def tcp_client_init(ip_addr, port):
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.connect((ip_addr, port))
	return s

def tcp_send_int(sock, x):
	print("Sending {0}".format(x))
	sock.send(struct.pack("!i", x))

def tcp_recv_int(sock):
	x = struct.unpack("!i", sock.recv(4))[0]
	print("Received {0}".format(x))
	return x

if __name__ == "__main__":
    # Get the IP address or hostname of the Linux server
    server_host = "10.152.0.99"
    server_port = 12397
    try:
        client_socket = tcp_client_init(server_host,server_port)

        list_1 = [int(integer) for integer in input("Enter a list of integers: ").split()]
        list_2 = [int(integer) for integer in input("Enter a list of integers: ").split()]

        tcp_send_int(client_socket,len(list_1))
        for integer in list_1:
            tcp_send_int(client_socket, integer)

        tcp_send_int(client_socket,len(list_2))
        for integer in list_2:
            tcp_send_int(client_socket, integer)

        list_3 = []

        len_list_3 = tcp_recv_int(client_socket)

        for _ in range(len_list_3):
            list_3.append(tcp_recv_int(client_socket))

        print(list_3)
        # Close the client socket
        client_socket.close()


    except socket.error as e:
        print(e)
