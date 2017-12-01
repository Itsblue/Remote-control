
/*
   Dec 2014 - TMRh20 - Updated
   Derived from examples by J. Coliz <maniacbug@ymail.com>
*/
/**
 * Example for efficient call-response using ack-payloads 
 * 
 * This example continues to make use of all the normal functionality of the radios including 
 * the auto-ack and auto-retry features, but allows ack-payloads to be written optionlly as well. 
 * This allows very fast call-response communication, with the responding radio never having to 
 * switch out of Primary Receiver mode to send back a payload, but having the option to switch to 
 * primary transmitter if wanting to initiate communication instead of respond to a commmunication. 
 */
 
#include <Servo.h>
#include <SPI.h>
#include "RF24.h"
#include <OneWire.h> 
#include <DallasTemperature.h>

#include "transceiver.h"

// Create variables to control servo value
Servo servo;
Servo motor;

/********************************************************************/
// Setup a oneWire instance to communicate with any OneWire devices  
OneWire oneWire(ONE_WIRE_BUS); 
/********************************************************************/
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
int nrtempssensors = 0;
/********************************************************************/ 

/****************** User Config for NRF24***************************/
/***      Set this radio as radio number RADIO0 or RADIO1         ***/
radio_type_e radioNumber = RADIO0;  //---> RADIO1 has the temperature sensor connected 

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(RF24_CNS,RF24_CE);
/**********************************************************/
                                                                                      // Topology
byte addresses[][6] = {"1Node","2Node"};                                              // Radio pipe addresses for the 2 nodes to communicate.

// Role management: Set up role.  This sketch uses the same software for all the nodes
// in this system.  Doing so greatly simplifies testing. Remote control and Remote client are defined
typedef enum { control = 1, client } role_e;                                          // The various roles supported by this sketch
const char* role_friendly_name[] = { "invalid", "Remote control", "Remote client"};   // The debug-friendly names of those roles
role_e role = client;                                                                 // The role of the current running sketch

byte counter = 1;                                                                     // A single byte to keep track of the data being sent back and forth
unsigned long key_control;                                                            // this is a randomized key for the first transmission from control to client
unsigned long key_client;                                                             // this is a randomized key for the second transmission from the client to control
unsigned long key_communication;                                                       // this is the fina key for the whole communication afterwards
transcv_s radio_data;

void setup(){

  Serial.begin(115200);

  // Get the radio number as set by the radio select pin - RADIO0 is default
  pinMode(RADIO_SEL, INPUT);
  if(digitalRead(RADIO_SEL) == HIGH){
    radioNumber = RADIO1; 
  }
  Serial.print(F("The current node is set to radio"));
  Serial.print(radioNumber);
  Serial.print(F(" as set by digital input pin "));
  Serial.println(RADIO_SEL);
  
  // Setup and configure radio

  // radio setup ...
  radio.begin();
  radio.setRetries(15, 15);               //the first is the time between reties in multiple of 250ms, the second is the numer of attempts
  //radio.enableAckPayload();                     // Allow optional ack payloads
  //radio.enableDynamicPayloads();                // Ack payloads are dynamic payloads
  
  if(radioNumber == RADIO0){
    radio.openWritingPipe(addresses[1]);        // Both radios listen on the same pipes by default, but opposite addresses
    radio.openReadingPipe(1,addresses[0]);      // Open a reading pipe on address 0, pipe 1
  }else{
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1,addresses[1]);
  }
  radio.startListening();                       // Start listening  
  
  //radio.writeAckPayload(1,&radio_data,sizeof(radio_data));          // Pre-load an ack-paylod into the FIFO buffer for pipe 1
  //radio.printDetails();

  // Start up the sensor library 
  sensors.begin(); 
  if(sensors.getDeviceCount()==0){
    role = control;                                 //only the client has a temperature sensor so far
  }

}

void loop(void) {

  
/****************** Ping Out Role ***************************/

  if (role == control){                               // Radio is in ping mode

    radio.stopListening();                                  // First, stop listening so we can talk.      

    unsigned long time = micros();                          // Record the current microsecond count   

    //set data to be send
    radio_data.counter = counter;
    radio_data.time = time;
    //Serial.print(F("Now sending datastructure of "));                         // Use a simple byte counter as payload
    //Serial.print(sizeof(radio_data)); 
    //Serial.print(F(" bytes. Value of counter: "));                        
    //Serial.println(radio_data.counter);

                                       
    if (!radio.write(&radio_data,sizeof(radio_data) )){                     // Send the counter variable to the other radio 
      Serial.println(F("Failed to send data to client"));
      //add error handling here if clients is not receiving data
    }
        
    radio.startListening();                                    // Now, continue listening

    unsigned long started_waiting_at = micros();               // Set up a timeout period, get the current microseconds
    boolean timeout = false;                                   // Set up a variable to indicate if a response was received or not
    transcv_s client_data;
    
    while ( ! radio.available() ){                             // While nothing is received
      if (micros() - started_waiting_at > 500000 ){            // If waited longer than 200ms, indicate timeout and exit while loop
          timeout = true;
          break;
      }      
    }
        
    if ( timeout ){                                             // Describe the results
        Serial.println(F("Failed, response timed out."));
        //add error handling here if client does not send response in time
    }else{
        radio.read( &client_data, sizeof(client_data) );                  // Read it, and display the response time
        unsigned long timer = micros();
        //Serial.print(F("Got response counter:"));
        //Serial.print(client_data.counter);
        //Serial.print(F(" time: "));
        //Serial.println(client_data.time);  
        Serial.print(F(" temp on client: "));
        Serial.println(client_data.temperature);  
        //Serial.print(F("Round-trip delay: "));
        //Serial.print(timer-client_data.time);
        //Serial.println(F(" microseconds"));
        counter++;                                  // Increment the counter variable
    
    }

    delay(1000);  // Try again later
  }


/****************** Pong Back Role ***************************/

  if ( role == client ) {
    byte pipeNo;                                   // Declare variables for the pipe and the byte received
    transcv_s control_data;
    
    sensors.requestTemperatures();                // read the sensor(s)
    Serial.print(F("Temperature is: "));
    Serial.println(sensors.getTempCByIndex(0));
    radio_data.temperature = sensors.getTempCByIndex(0);                    // get the data
    
    if( radio.available(&pipeNo)){
      while( radio.available(&pipeNo)){              // Read all available payloads
        radio.read( &control_data, sizeof(control_data) );     
      }              
      
      radio.stopListening();                               // First, stop listening so we can talk 
      
      Serial.print(F("Got remote data counter: "));
      Serial.print(control_data.counter); 
      Serial.print(F(" time: "));
      Serial.println(control_data.time);  
      radio_data.counter = control_data.counter + 1;                                // Ack payloads are much more efficient than switching to transmit mode to respond to a call
      radio_data.time = control_data.time;
      Serial.print(F("Loaded next response "));
      Serial.println(radio_data.counter);  

      if( radio.write(&radio_data, sizeof(radio_data) )){
        Serial.println(F("Sending failed."));           // If no ack response, sending failed
        //add error handling here if remote does not recieve the data anymore
      }

      radio.startListening();
      
   }
 }

}
