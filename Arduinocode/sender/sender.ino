/*
 * Sender ... this Code is for the Arduino Nano ... for another device the pins have to be changed
 */


#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "sender.h"

RF24 radio(RF24_CNS,RF24_CE); // CNS, CE

boolean buttonState = 0;

const byte address[6] = "00001";

trans_data_s trans_data;

void setup() {
  pinMode(BUTTON, INPUT);
  Serial.begin(9600);
  Serial.print("starting up radio");
  radio.begin();
  Serial.println("...done");
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {
    buttonState = digitalRead(BUTTON);
    Serial.print("Button:");
    Serial.println(buttonState);

    //set and send data
    trans_data.trans = HNDSHK;
    trans_data.hndshk_sig = 0;
    radio.write(&trans_data, sizeof(trans_data));
    delay(5);
}
