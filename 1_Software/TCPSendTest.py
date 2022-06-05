import socket
TCP_IP = "110.164.204.27"
TCP_PORT = 22890
MESSAGE = b"Hello, World!"

print("UDP target IP: %s" % TCP_IP)
print("UDP target port: %s" % TCP_PORT)
print("message: %s" % MESSAGE)

sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_STREAM) # TCP
sock.connect((TCP_IP, TCP_PORT))
sock.send(MESSAGE)
