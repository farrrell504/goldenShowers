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
x=1
msg.add_arg(str(pow(2,x)))
msg = msg.build()
client.send(msg)

#65535 is all on
#msg.add_arg('3.0')
#msg = msg.build()
#client.send(msg)
