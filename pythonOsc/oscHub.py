#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Nov  3 22:48:43 2017

@author: farrrell504
"""

"""Small example OSC server

This program listens to several addresses, and prints some information about
received packets.
"""
import argparse
import math

from pythonosc import dispatcher
from pythonosc import osc_server
from pythonosc import osc_message_builder
from pythonosc import udp_client

hostComputerIP='192.168.1.12';
hostComputerPort=7563;

remoteBelatIP='192.168.1.18';
remoteBelaPort=7562;

client = udp_client.UDPClient(remoteBelatIP, remoteBelaPort)

def print_ack_handler(unused_addr, args, volume):
  print("[{0}] ~ {1}".format(args[0], volume))


def reply_osc_startup(unused_addr, args, volume):
  print("[{0}] ~ {1}".format(args[0], volume))
  msg = osc_message_builder.OscMessageBuilder(address = '/osc-setup-reply')
  msg = msg.build()
  client.send(msg)

def print_compute_handler(unused_addr, args, volume):
  try:
    print("[{0}] ~ {1}".format(args[0], args[1](volume)))
  except ValueError: pass

if __name__ == "__main__":
  parser = argparse.ArgumentParser()
  parser.add_argument("--ip",
      default=hostComputerIP, help="The ip to listen on")
  parser.add_argument("--port",
      type=int, default=hostComputerPort, help="The port to listen on")
  args = parser.parse_args()

  dispatcher = dispatcher.Dispatcher()
  dispatcher.map("/filter", print)
  dispatcher.map("/ack", print_ack_handler, "Acknowledgment")
  dispatcher.map("/logvolume", print_compute_handler, "Log volume", math.log)
  dispatcher.map("/osc-setup", reply_osc_startup, "Startup")

  server = osc_server.ThreadingOSCUDPServer(
      (args.ip, args.port), dispatcher)
  print("Serving on {}".format(server.server_address))
  server.serve_forever()

