/*  Code to receive data from RF24L01+ and use it to control a servo */
#include <Servo.h>
#include <SPI.h>
#include <RF24.h>
#include "receiver.h";

//Radio Configuration
//bool radioNumber=0;
RF24 radio(RF24_CNS, RF24_CE);
byte addresses[6] = {"rc001"};              // 5 letter address of this device 
//bool role = 0;  //Control transmit/receive

// Create variables to control servo value

Servo servo;
Servo motor;
int valserv;       // Expected range 0 - 180 degrees
int valmot;     // Expected range 0 - 180 degrees

cmd_s cmd;
cmd_s hnd_shk_data;
data_type_e next_cmd = HNDSHK;
data_type_e act_cmd = HNDSHK;

boolean recvd_correct_cmd(data_type_e next_cmd, data_type_e recvd_cmd);

void setup() {
  
  //servo.attach(D8);         // Plug a servo signal line into digital output pin 6
  //motor.attach(D2);       // Plug a servo signal line into digital output pin 7
  
  Serial.begin(9600);       // Send data back for debugging purposes
  
  radio.begin();                          //start the radio
  radio.setRetries(15, 15);               //the first is the time between reties in multiple of 250ms, the second is the numer of attempts
  radio.setPALevel(RF24_PA_MIN);          //set the level
  radio.openWritingPipe(addresses);       //open the writing pipe
  radio.openReadingPipe(0, addresses);    //open the reading pipe at stream 0
  radio.startListening();                 //start the listning mode

  Serial.print("Size of our data structure has to be not more than 32byte - it is:");
  Serial.println(sizeof(trans_data));

  if(sizeof(trans_data)>32){
    Serial.println("Size of datastructure to receive is to big.... ERROR!");
  }

}

void loop() {
  
  delay(10);

  
  if(radio.available()){                              //there are new data ...
    radio.read(&trans_data,sizeof(trans_data_s));     //read it ...

    if(recvd_correct_cmd(next_cmd, cmd.type) == false){

      switch(cmd.type){
        case HNDSHK:
          //we need to take the data invert it and send it back to sender ... afterwards waiting for new transmission
          handle_handshake(&cmd, &hnd_shk_data, &next_cmd, &act_cmd);
        break;
        case BUT:
          //do everything that has to be done for the button
        break;
      }

    }
    // handle fail - send message to master that we got a wrong command


    if(trans_data.trans == HNDSHK){
         Serial.print("Handshake read.");
         Serial.print(" Value is:");
         Serial.println(trans_data.hndshk_sig);

         Serial.print("Button state is:");
         Serial.println(trans_data.but.state);
         
         //radio.read(&valserv,sizeof(valserv));
         //radio.read(&valmot,sizeof(valmot));
         //if(valserv && valmot > 0){
          
         //Serial.print(valserv);
         //Serial.print("\t");
         //Serial.println(valmot);
         //servo.write(valserv);
         //motor.write(valmot);
         
         //}
      }
  } else {Serial.println("No radio");}
  
}

/**********************************************************************************
 * FUNCTIONS
 **********************************************************************************/

boolean handle_handshake(cmd_s* cmd, cmd_s* hnd_shk_data, data_type_e* next_cmd, data_type_e* act_cmd)
{
  boolean rc = true;
  // set the next_cmd based on what the handshakes says and store the handshake data - that is exactly what we will have to use with the next cmnd and invert the integer data ...



  return(rc);
}

boolean recvd_correct_cmd(data_type_e next_cmd, data_type_e recvd_cmd)
{
  boolean rc = true;
  if(next_cmd != recvd_cmd)
  {
    Serial.print("The command sent by the master is not what is expected - as sent by the handshake: ");
    Serial.print(" Expected was ");
    Serial.print(next_cmd);
    Serial.print(" Received ");
    Serial.println(recvd_cmd);
    rc = false;
  }
  else
  {
    Serial.print("The command sent by the master is as expected: ");
    Serial.println(next_cmd);
    rc = true;
  }

  return(rc);
}

