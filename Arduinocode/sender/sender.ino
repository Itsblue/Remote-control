/*
 * Sender ... this Code is for the Arduino Nano ... for another device the pins have to be changed
 */


#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <stdint.h>
#include "sender.h"

RF24 radio(RF24_CNS,RF24_CE); // CNS, CE

boolean buttonState = 0;

const byte address[6] = "rc001";      // can be any 5 letter string

trans_data_s trans_data;              //max is 32 bytes

void setup() {
  pinMode(BUTTON, INPUT);
  Serial.begin(9600);
  Serial.print("starting up radio");
  radio.begin();                      //start the readio
  radio.setRetries(15, 15);           //the first is the time between reties in multiple of 250ms, the second is the numer of attempts
  Serial.println("...done");
  radio.openWritingPipe(address);     //this is the address of the reciever
  radio.setPALevel(RF24_PA_MIN);      //Level of radio
  radio.stopListening();              //stop listing - start sending
}

void loop() {
    buttonState = digitalRead(BUTTON);
    Serial.print("Button:");
    Serial.println(buttonState);

    //set and send data
    trans_data.trans = HNDSHK;
    trans_data.hndshk_sig = 3;
    trans_data.but.state = buttonState;
    Serial.println(sizeof(trans_data));
    Serial.println(sizeof(trans_data.hndshk_sig));
    Serial.println(sizeof(trans_data.trans));
    radio.write(&trans_data, sizeof(trans_data_s)); // the write will try all the attempts or until it was able to sent the data - if the data was send successfully it will give True in the other case a False
    delay(5);
}
