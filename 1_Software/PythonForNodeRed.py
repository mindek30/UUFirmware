import socket
TCP_IP = "110.164.204.27"
TCP_PORT = 22890

print(type(msg))
print(msg["payload"])
print(msg["topic"])
print(msg["_msgid"])

MESSAGE = bytearray(str(msg["payload"]), 'utf-8')
print("UDP target IP: %s" % TCP_IP)
print("UDP target port: %s" % TCP_PORT)
print("message: %s" % MESSAGE)

sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_STREAM) # TCP
sock.connect((TCP_IP, TCP_PORT))
sock.send(MESSAGE)

msgOutput =	{
  "topic": "aisud",
  "payload": 1964
}
#return  [msg, msgOutput] # return muti
return  msg