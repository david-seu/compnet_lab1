import struct
import socket


def tcp_send_uint(sock, x):
    print("Sending {0}".format(x))
    sock.send(struct.pack("!H", x))


def tcp_recv_float(sock):
    x = struct.unpack("!f", sock.recv(4))[0]
    print("Received {0}".format(x))
    return x


def tcp_recv_uint(sock):
    x = struct.unpack("!H", sock.recv(2))[0]
    print("Received {0}".format(x))
    return x


def tcp_send_float(sock, x):
    print("Sending {0}".format(x))
    sock.send(struct.pack("!f", x))


def tcp_client_init(ip_addr, port):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((ip_addr, port))
    return s


if __name__ == "__main__":
    client_socket = tcp_client_init("172.19.1.199", 1234)
    n = int(input("Enter float vector length; "))
    tcp_send_uint(client_socket, n)
    if n != 0:
        l = [float(x) for x in input(f"Enter {n} float numbers: ").split()]
        for x in l:
            tcp_send_float(client_socket, x)
    n = tcp_recv_uint(client_socket)
    l = []
    for _ in range(n):
        l.append(tcp_recv_float(client_socket))
    print("List is:", l)