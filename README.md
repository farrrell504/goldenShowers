# goldenShowers

## If using the dummy python scripts to simulate OSC messages
  1. Run oscHub.py first
    1. You'll need to modify the IP address to be that of your computers
  2. Point browser at Bela IDE
    1. Copy in the render.cpp text
    2. Change remoteIP to this same IP address
    3. Run the project
      1. Should see a message saying handshake received
  3. Run oscSend.py as desired
    1. Change the message sent to check for Bela correctly receiving, decoding the message, and transmitting the message back to your pc
    2. Terminal running oscHub should show the message you sent being received back
  
  
