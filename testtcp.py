import socket
TCP_IP = "27.254.115.167"
TCP_PORT = 1333
MESSAGE = b"Check Haha"

print("UDP target IP: %s" % TCP_IP)
print("UDP target port: %s" % TCP_PORT)
print("message: %s" % MESSAGE)

sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_STREAM) # TCP
sock.connect((TCP_IP, TCP_PORT))
sock.send(MESSAGE)
