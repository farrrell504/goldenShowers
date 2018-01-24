# goldenShowers

If using the dummy python scripts to simulate OSC messages
  Run oscHub.py first
    You'll need to modify the IP address to be that of your computers
  Point browser at Bela IDE
    Copy in the render.cpp text
    Change remoteIP to this same IP address
    Run the project
      Should see a message saying handshake received
  Run oscSend.py as desired
    Change the message sent to check for Bela correctly receiving, decoding the message, and transmitting the message back to your pc
    Terminal running oscHub should show the message you sent being received back
  
  
