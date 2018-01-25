/*
 ____  _____ _        _    
| __ )| ____| |      / \   
|  _ \|  _| | |     / _ \  
| |_) | |___| |___ / ___ \ 
|____/|_____|_____/_/   \_\

The platform for ultra-low latency audio and sensor processing

http://bela.io

A project of the Augmented Instruments Laboratory within the
Centre for Digital Music at Queen Mary University of London.
http://www.eecs.qmul.ac.uk/~andrewm

(c) 2016 Augmented Instruments Laboratory: Andrew McPherson,
  Astrid Bin, Liam Donovan, Christian Heinrichs, Robert Jack,
  Giulio Moro, Laurel Pardue, Victor Zappi. All rights reserved.

The Bela software is distributed under the GNU Lesser General Public License
(LGPL 3.0), available here: https://www.gnu.org/licenses/lgpl-3.0.txt
*/


#include <Bela.h>
#include <OSCServer.h>
#include <OSCClient.h>

#define VALVE(idx) (1<<idx)

OSCServer oscServer;
OSCClient oscClient;

// this example is designed to be run alongside resources/osc/osc.js

// parse messages received by OSC Server
// msg is Message class of oscpkt: http://gruntthepeon.free.fr/oscpkt/
int parseMessage(oscpkt::Message msg){
    
    rt_printf("received message to: %s\n", msg.addressPattern().c_str());
   
    int intArg=2;
	std::string s;
    if (msg.match("/osc-test").popStr(s)){
       intArg=std::stoi( s );
       rt_printf("maybe %i\n",intArg);
    }
    else{
    	intArg = 1;
    	rt_printf("received shit");
    }
    return intArg;
}

int localPort = 7562;
int remotePort = 7563;
const char* remoteIp = "192.168.1.12";

int belaNumber = 1; //will be one of 17

bool setup(BelaContext *context, void *userData)
{
    oscServer.setup(localPort);
    oscClient.setup(remotePort, remoteIp);
    
    // the following code sends an OSC message to address /osc-setup
    // then waits 1 second for a reply on /osc-setup-reply
    
    bool handshakeReceived = false;
    oscClient.sendMessageNow(oscClient.newMessage.to("/osc-setup").add(belaNumber).end());
    oscServer.receiveMessageNow(1000);
    while (oscServer.messageWaiting()){
        if (oscServer.popMessage().match("/osc-setup-reply")){
            handshakeReceived = true;
        }
    }
    
    if (handshakeReceived){
        rt_printf("handshake received!\n");
    } else {
        rt_printf("timeout!\n");
    }
    
	return true;
}

void render(BelaContext *context, void *userData)
{
    // receive OSC messages, parse them, and send back an acknowledgment
    while (oscServer.messageWaiting()){
        int count = parseMessage(oscServer.popMessage());
        rt_printf("%i\n",count);
        for(int k=0;k<8;k++){
        	if(count&VALVE(k)){
        		rt_printf("turn that shit on valve %i\n",k);
        	} else {
        		rt_printf("no one wants you on valve %i\n",k);
        	}
        }
        oscClient.queueMessage(oscClient.newMessage.to("/volume").add(count).end()); //lets main controller know message was correctly received
    }
}

void cleanup(BelaContext *context, void *userData)
{

}


/**
\example OSC/render.cpp

Open Sound Control
------------------

This example shows an implementation of OSC (Open Sound Control) which was 
developed at UC Berkeley Center for New Music and Audio Technology (CNMAT).

It is designed to be run alongside resources/osc/osc.js

The OSC server port on which to receive is set in `setup()` 
via `oscServer.setup()`. Likewise the OSC client port on which to 
send is set in `oscClient.setup()`.

In `setup()` an OSC message to address `/osc-setup`, it then waits 
1 second for a reply on `/osc-setup-reply`.

in `render()` the code receives OSC messages, parses them, and sends 
back an acknowledgment.
*/
