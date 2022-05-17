import socket
import sys

# Create a UDP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

server_address = ('192.168.101.103', 6681)
message = b'This is the message.  It will be repeated.'

try:

    # Send data
    print('sending {!r}'.format(message))
    sent = sock.sendto(message, server_address)

    # Receive response
    print('waiting to receive')
    data, server = sock.recvfrom(4096)
    print('received {!r}'.format(data))

finally:
    print('closing socket')
    sock.close()