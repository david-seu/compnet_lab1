import socket
import select
import struct
import sys

def tcp_send_int(sock, x):
    try:
        print("Sending {0}".format(x))
        sock.send(struct.pack("!i", x))
    except socket.error as e:
        print(e)
        exit(-1)


def tcp_client_init(ip_addr, port):
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((ip_addr, port))
        return s
    except socket.error as e:
        print(e)
        exit(-1)


def tcp_recv_string(sock):
    try:
        string = sock.recv(1024).decode('ascii')
        print("Received {0}".format(string))
        return string
    except socket.error as e:
        print(e)
        exit(-1)


if __name__ == "__main__":
    client_socket = tcp_client_init('localhost', 8080)
    master = [client_socket, sys.stdin]
    while True:
        read_fds = master

        ready_to_read, _, _ = select.select(read_fds, [], [])

        if sys.stdin in ready_to_read:
            number = int(input("Enter a number: "))
            tcp_send_int(client_socket,number)

        elif client_socket in ready_to_read:
            answer = tcp_recv_string(client_socket)
            print(f"Server answer is: {answer}")
            if answer == '0':
                break
    try:
        client_socket.close()
    except socket.error as e:
        print(e)
        exit(-1)