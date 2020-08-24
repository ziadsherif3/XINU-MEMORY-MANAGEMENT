import socket

class PageServer:
    
    def __init__(self):
        self.backing_store = {}
    
    def handle_request(self, msg: str):
        pass

def setup_socket():
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind(("192.168.1.42", 49188))
    return sock

def main():
    """
    Setup the UDP socket and the page server
    """
    sock = setup_socket()
    server = PageServer()
    
    """
    Receive requests
    """
    while True:
        data, addr = sock.recvfrom(4096)
        server.handle_request(data)

if __name__ == "__main__":
    main()