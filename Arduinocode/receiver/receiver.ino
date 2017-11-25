/*  Code to receive data from RF24L01+ and use it to control a servo */
#include <Servo.h>
#include <SPI.h>
#include <RF24.h>
#include "receiver.h";

//Radio Configuration
//bool radioNumber=0;
RF24 radio(D4, D3);
byte addresses[6] = "00001";
//bool role = 0;  //Control transmit/receive

// Create variables to control servo value

Servo servo;
Servo motor;
int valserv;       // Expected range 0 - 180 degrees
int valmot;     // Expected range 0 - 180 degrees

trans_data_s trans_data;

void setup() {
  
  servo.attach(D8);         // Plug a servo signal line into digital output pin 6
  motor.attach(D2);       // Plug a servo signal line into digital output pin 7
  
  Serial.begin(9600);       // Send data back for debugging purposes
  
  radio.begin();
  radio.setPALevel(RF24_PA_MIN);
  //radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(0, addresses);
  radio.startListening();  
}

void loop() {

  delay(10);
  if(radio.available()){
    radio.read(&trans_data,sizeof(trans_data_s));
    Serial.println(sizeof(trans_data));
    Serial.println(sizeof(trans_data.hndshk_sig));
    Serial.println(sizeof(trans_data.trans));

    if(trans_data.trans == HANDSHAKE){
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
