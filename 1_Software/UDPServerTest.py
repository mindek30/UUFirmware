import socket
import sys
import threading

address = 0
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

def thread_callback_1():
    global address
    global sock
    # Bind the socket to the port
    server_address = ('', 1001)
    print('starting up on {} port {}'.format(*server_address))
    sock.bind(server_address)

    while True:
        print('\nwaiting to receive message')
        data, address = sock.recvfrom(4096)

        print('received {} bytes from {}'.format(len(data), address))
        print(data)

#        if data:
#            sent = sock.sendto(data, address)
#            print('sent {} bytes back to {}'.format(sent, address))

#def thread_callback_2():
#    global address
#    global sock
#    while True:
#        val = input("Enter your value: ")
#        sent = sock.sendto( val.encode(), address)

x = threading.Thread(target=thread_callback_1)
#y = threading.Thread(target=thread_callback_2)

x.start()
#y.start()
