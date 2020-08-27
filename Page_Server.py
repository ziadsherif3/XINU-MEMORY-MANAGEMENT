import socket
import struct
import enum

class PageServer:
    
    class Constants(enum.Enum):
        PAGESERVERERR = 0
        OK = 1
    
    class PacketType(enum.Enum):
        GETBS = 0
        RELEASEBS = 1
        READBS = 2
        WRITEBS = 3
    
    def __init__(self):
        self.backing_store = {}
        self.packet_buffer = []
    
    def has_pending_packets(self):
        return len(self.packet_buffer) != 0
    
    def get_next_output_packet(self):
        return self.packet_buffer.pop(0)
    
    def handle_request(self, data):
        """
        Extract the opcode
        """
        (opcode,), data = struct.unpack("<I", data[:4]), data[4:]
        
        if opcode == PageServer.PacketType.GETBS.value:
            self.get_bs_response(data)
            
        elif opcode == PageServer.PacketType.RELEASEBS.value:
            self.release_bs_response(data)
            
        elif opcode == PageServer.PacketType.READBS.value:
            self.read_bs_response(data)
            
        elif opcode == PageServer.PacketType.WRITEBS.value:
            self.write_bs_response(data)
    
    def get_bs_response(self, data):
        bsd_t_ID, npages = struct.unpack("<II", data)
        
        if npages == 0:
            outdata = struct.pack("<I", PageServer.Constants.PAGESERVERERR.value)
        else:
            if bsd_t_ID not in self.backing_store:
                self.backing_store[bsd_t_ID] = {"size": npages}
        
            size = self.backing_store[bsd_t_ID]["size"]
            outdata = struct.pack("<II", PageServer.Constants.OK.value, size)
        
        self.packet_buffer.append(outdata)
        
    
    def release_bs_response(self, data):
        (bsd_t_ID,) = struct.unpack("<I", data)
        
        if bsd_t_ID not in self.backing_store:
            outdata = struct.pack("<I", PageServer.Constants.PAGESERVERERR.value)
        else:
            del self.backing_store[bsd_t_ID]
            outdata = outdata = struct.pack("<I", PageServer.Constants.OK.value)
        
        self.packet_buffer.append(outdata)
    
    def read_bs_response(self, data):
        pass
    
    def write_bs_response(self, data):
        (bsd_t_ID, pagenum, division), data = struct.unpack("<III", data[:12]), data[12:]
        
        if bsd_t_ID not in self.backing_store:
            outdata = struct.pack("<I", PageServer.Constants.PAGESERVERERR.value)
        else:
            if pagenum not in self.backing_store[bsd_t_ID]:
                self.backing_store[bsd_t_ID][pagenum] = {}
            
            division_data = struct.unpack("<256I", data)
            self.backing_store[bsd_t_ID][pagenum][division] = division_data
            current_size = len(self.backing_store[bsd_t_ID]) - 1
            
            if (division == 1) and (self.backing_store[bsd_t_ID]["size"] == current_size):
                self.backing_store[bsd_t_ID]["size"] += 1
            
            outdata = outdata = struct.pack("<I", PageServer.Constants.OK.value)
        
        self.packet_buffer.append(outdata)
        
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
        if server.has_pending_packets:
            sock.sendto(server.get_next_output_packet(), addr)

if __name__ == "__main__":
    main()