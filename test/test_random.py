import socket
import time
from random import randint

UDP_IP = "bedled-server"
UDP_PORT = 2807

print("UDP target IP:", UDP_IP)
print("UDP target port:", UDP_PORT)

sock = socket.socket(socket.AF_INET, # Internet
                    socket.SOCK_DGRAM) # UDP
while True:
    time.sleep(1.0 / 10)
    buf = bytearray('s', encoding='ascii')
    for i in range(0, 60):
        buf += bytearray((randint(0, 255), randint(0, 255), randint(0, 255)))
    sock.sendto(buf, (UDP_IP, UDP_PORT))