import socket


class Listener:
    def __init__(self, host, port):

        # Set the IP address and port number for the receiver (use your own IP address here or 'localhost')
        self.host = host
        self.port = port

        # Create a UDP socket
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

        # Bind the socket to the address and port
        self.sock.bind((host, port))

    def listen_step(self):
        print(f"Listening on {self.host}:{self.port}...")

        data, address = self.sock.recvfrom(1024)  # Buffer size is 1024 bytes

        print(data.decode(), address)

        return data.decode()

