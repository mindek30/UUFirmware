import socket
import sys

# Create a UDP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Bind the socket to the port
server_address = ('', 1880)
print('starting up on {} port {}'.format(*server_address))
sock.bind(server_address)

while True:
    print('\nwaiting to receive message')
    data, address = sock.recvfrom(4096)

    print('received {} bytes from {}'.format(len(data), address))
    print(data.hex())

    if data:
        hex_string = "deadbeef"
        arr = bytearray.fromhex(hex_string)
        sent = sock.sendto(arr, address)
        #sent = sock.sendto(data, address)
        print('sent {} bytes back to {}'.format(sent, address))
