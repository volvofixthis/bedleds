import socket
import time
from random import randint

UDP_IP = "bedled-server2"
UDP_PORT = 2807

print("UDP target IP:", UDP_IP)
print("UDP target port:", UDP_PORT)

sock = socket.socket(socket.AF_INET, # Internet
                    socket.SOCK_DGRAM) # UDP
buf = bytearray('h', encoding='ascii')
for i in range(0, 60):
    buf += bytearray((randint(0, 255), 0, 0))
sock.sendto(buf, (UDP_IP, UDP_PORT))
