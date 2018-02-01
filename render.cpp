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

int pin[16]{
	P8_07, //pin1
	P8_08, //1
	P8_09, //2
    P8_10, //3
    P8_11, //4
    P8_12, //5
    P9_12, //6
    P9_14, //7
    P8_15, //8
    P8_16, //9
    P9_16, //10
    P8_18, //11
    P8_27, //12
    P8_28, //13
    P8_29, //14
	P8_30}; //15
	
unsigned int valveStatus = 0b0000000000000000; 
//holds status for the 16 valves. 
//1 is on. 0 is off.

OSCServer oscServer;
OSCClient oscClient;

int localPort = 7562;
int hostComputerPort = 7563;
const char* hostComputerIp = "192.168.1.12";

int belaNumber = 1; //will be one of 17

int parseMessage(oscpkt::Message msg){
    
    //rt_printf("received message to: %s\n", msg.addressPattern().c_str());
   
    int intArg=2;
	std::string s;
    if (msg.match("/osc-test").popStr(s)){
       intArg=std::stoi( s );
       //rt_printf("maybe %i\n",intArg);
    } else if (msg.match("/whoisthis").popStr(s)){
    	intArg=131071;
    	//could use s as the sender ID? not sure if useful
    	rt_printf("whoisthis received",intArg);
    }
    else{
    	intArg = 1;
    	rt_printf("received shit");
    }
    return intArg;
}



bool setup(BelaContext *context, void *userData)
{
    oscServer.setup(localPort);
    oscClient.setup(hostComputerPort, hostComputerIp);
    
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
    
    for(int k=0; k<16; k++) {
    pinMode(context, 0, pin[k], OUTPUT);
    digitalWrite(context, 0, pin[k], 0);
    }
    
    
    
	return true;
}

void render(BelaContext *context, void *userData)
{
    // receive OSC messages, parse them, and send back an acknowledgment
    while (oscServer.messageWaiting()){
        int newValveValue = parseMessage(oscServer.popMessage());
        if (newValveValue==131071){
        	oscClient.queueMessage(oscClient.newMessage.to("/whoiam").add(belaNumber).end());
        }	else{
	        //rt_printf("%i\n",newValveValue ); //150 is 10010110
	        for(int k=0;k<16;k++){
	        	if((newValveValue &VALVE(k)) != (valveStatus&VALVE(k))){ //want to say if != currentStatus 
	        		digitalWrite(context, 0, pin[k], (newValveValue &VALVE(k)));
	        		
	        		//rt_printf("change yo shit up valve %i, valve(k) %i, valveStatus %u, value told %i, compared against%i\n",k,VALVE(k),valveStatus,(newValveValue &VALVE(k)),(valveStatus&VALVE(k)));
	        		//rt_printf("xor of %i to %i\n",VALVE(k),valveStatus);
	        		valveStatus = VALVE(k)^valveStatus;
	        		//rt_printf("valve status is now %i\n",valveStatus);
	        	} else {
	        		//rt_printf("you're perfect just the way you are valve %i\n",k);
	        	}
	        }
	        oscClient.queueMessage(oscClient.newMessage.to("/ack").add(newValveValue).end()); //lets main controller know message was correctly received
        }
    }
}

void cleanup(BelaContext *context, void *userData)
{

}


/**
\
