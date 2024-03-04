import socket
import struct


def tcp_client_init(ip_addr, port):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((ip_addr, port))
    return s

def tcp_send_uint(sock, x):
    try:
        print("Sending {0}".format(x))
        sock.send(struct.pack("!H", x))
    except struct.error as e:
        print(e)
        exit(-1)

def tcp_recv_uint(sock):
    try:
        x = struct.unpack("!H", sock.recv(2))[0]
        print("Received {0}".format(x))
        return x
    except struct.error as e:
        print(e)
        exit(-1)


if __name__ == "__main__":
    try:
        client_udp = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)  # UDP
        client_udp.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        client_udp.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
        client_udp.bind(("", 37020))

        data = ""
        while data == "":
            data, addr = client_udp.recvfrom(1024)
            print(data)

        client_udp.close()

        questions = data.decode().split(";")
        client_tcp = tcp_client_init("192.168.8.115",1234)

        for q in questions:
            print(q)
            tcp_send_uint(client_tcp, int(input("Enter the answer: ")))

        score = tcp_recv_uint(client_tcp)
        print("Your score is: ", score)
        client_tcp.close()

    except socket.error as e:
        print(e)
        exit(-1)