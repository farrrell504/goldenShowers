#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Nov  3 22:15:07 2017

@author: farrrell504
"""

from pythonosc import osc_message_builder
from pythonosc import udp_client
client = udp_client.UDPClient('192.168.1.18', 7562)
msg = osc_message_builder.OscMessageBuilder(address = '/osc-test')
msg.add_arg('150')
#msg.add_arg('3.0')
msg = msg.build()
client.send(msg)
