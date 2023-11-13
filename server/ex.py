import socket

# Create a socket object
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Define the port on which you want to listen
port = 12397

# Bind the socket to the address
server_socket.bind(("0.0.0.0", port))

# Listen for incoming connections (max 5 clients in the queue)
server_socket.listen(5)

while True:
    # Accept a connection from the client
    client_socket, addr = server_socket.accept()

    print(f"Received connection from {addr}")

    # Send a welcome message to the client
    message = "Welcome to the server!"
    client_socket.send(message.encode())

    # Close the client socket
    client_socket.close()
