#!/usr/bin/env python3

import paho.mqtt.client as mqtt
import win32com.client
import time

sp = win32com.client.Dispatch("SAPI.SpVoice")
# This is the Publisher

client = mqtt.Client()
#client.connect("192.168.101.103",1883,60)
client.connect("ekmsev.thddns.net",6680,60)
for i in range(25):
        client.publish("uucontrol", "A" + str(i) +"FFFF:FFFFFFFF:FFFFFFFFFFFFFFFF" )
        #print(str(i))
        #sp.Speak(str(i))
        #time.sleep(0.5)
print("stop")
client.disconnect()