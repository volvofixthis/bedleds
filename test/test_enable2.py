import socket
import time

UDP_IP = "bedled-server2"
UDP_PORT = 2807

print("UDP target IP:", UDP_IP)
print("UDP target port:", UDP_PORT)

sock = socket.socket(socket.AF_INET, # Internet
                    socket.SOCK_DGRAM) # UDP
sock.sendto(bytearray('c', encoding='ascii'), (UDP_IP, UDP_PORT))
