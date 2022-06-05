import socket
import sys
import threading

HOST = '' # Symbolic name, meaning all available interfaces
PORT = 1001 # Arbitrary non-privileged port
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
def thread_callback_1():
    global HOST
    global PORT
    s.bind((HOST, PORT))
    s.listen()
    while True:
        conn, addr = s.accept()
        with conn:
            print(f"Connected by {addr}")
            data = conn.recv(4096)
            if not data:
                continue
            print('received {} bytes from {}'.format(len(data), addr))
            print(data)
#        if data:
#            sent = sock.sendto(data, address)
#            print('sent {} bytes back to {}'.format(sent, address))

#def thread_callback_2():
#    while True:
#        val = input("Enter your value: ")
#        s.connect((TCP_IP, TCP_PORT))
#        sent = s.send( val.encode(), TCP_IP)

x = threading.Thread(target=thread_callback_1)
#y = threading.Thread(target=thread_callback_2)

x.start()
#y.start()
