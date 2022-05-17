import paho.mqtt.client as mqtt
bkserver = "127.0.0.1"
bkport = 1883
bkuser = "uuadmin"
bkpass = "uu2021"

bktopic = "test"

# This is the Subscriber

def on_connect(client, userdata, flags, rc):
  print("Connected with result code "+str(rc))
  client.subscribe(bktopic)

def on_message(client, userdata, msg):
	qosX = str(msg.qos)
	topicX = str(msg.topic)
	loadX = msg.payload.decode()
	print("topic = " + topicX + " qos = " + qosX + " msg = " + loadX)

client = mqtt.Client()
client.username_pw_set(bkuser,bkpass)
client.connect(bkserver,bkport,100)

client.on_connect = on_connect
client.on_message = on_message

client.loop_forever()
client.disconnect()
