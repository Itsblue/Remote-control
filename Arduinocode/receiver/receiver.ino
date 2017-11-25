/*  Code to receive data from RF24L01+ and use it to control a servo */

#include <Servo.h>
#include <SPI.h>
#include <RF24.h>

//Radio Configuration
bool radioNumber=0;
RF24 radio(D4, D3);
byte addresses[][6] = {"1Node","2Node"};
bool role = 0;  //Control transmit/receive
int start = 0;

// Create variables to control servo value

Servo servo;
Servo motor;
int valserv;       // Expected range 0 - 180 degrees
int valmot;     // Expected range 0 - 180 degrees

void setup() {
  
  servo.attach(D8);         // Plug a servo signal line into digital output pin 6
  motor.attach(D2);       // Plug a servo signal line into digital output pin 7
  
  Serial.begin(9600);       // Send data back for debugging purposes
  
  radio.begin();
  radio.setPALevel(RF24_PA_MAX);
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1,addresses[1]);
  radio.startListening();  
}

void loop() {

  delay(10);
  if(radio.available()){
    radio.read(&start,sizeof(start));
    if(start == 1){
         radio.read(&valserv,sizeof(valserv));
         radio.read(&valmot,sizeof(valmot));
         if(valserv && valmot > 0){
          
         Serial.print(valserv);
         Serial.print("\t");
         Serial.println(valmot);
         servo.write(valserv);
         motor.write(valmot);
         
         }
    }
    start = 0;
  } else {Serial.println("No radio");}
  

}
