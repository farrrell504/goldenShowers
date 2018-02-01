#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Nov  3 22:15:07 2017

@author: farrrell504
"""

import time

from pythonosc import osc_message_builder
from pythonosc import udp_client
client = udp_client.UDPClient('192.168.1.18', 7562)
msg = osc_message_builder.OscMessageBuilder(address = '/osc-test')
#msg = osc_message_builder.OscMessageBuilder(address = '/whoisthis')
x = 0
msg.add_arg('2')
msg = msg.build()
client.send(msg)

#attempt at testing throughput. my wifi likes pausing the stream every ~10.5s 
while x<65535:
    #time.sleep(0.01)
    #msg = osc_message_builder.OscMessageBuilder(address = '/osc-test')
    #msg.add_arg('0')
    #msg = msg.build()
    #client.send(msg)
    time.sleep(0.05)
    msg = osc_message_builder.OscMessageBuilder(address = '/osc-test')
    msg.add_arg(str(x))
    msg = msg.build()
    client.send(msg)
    x = x + 1

#tests all valves are working in the right order
while x<16:
    #time.sleep(0.01)
    #msg = osc_message_builder.OscMessageBuilder(address = '/osc-test')
    #msg.add_arg('0')
    #msg = msg.build()
    #client.send(msg)
    time.sleep(2)
    msg = osc_message_builder.OscMessageBuilder(address = '/osc-test')
    msg.add_arg(str(pow(2,x))
    msg = msg.build()
    client.send(msg)
    x = x + 1


#65535 is all on
#msg.add_arg('3.0')
#msg = msg.build()
#client.send(msg)

