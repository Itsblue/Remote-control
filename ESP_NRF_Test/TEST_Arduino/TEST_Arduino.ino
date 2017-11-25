/*
 * Sender
 */


#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define button 4

RF24 radio(7,8); // CNS, CE

boolean buttonState = 0;

const byte address[6] = "00001";

void setup() {
  pinMode(button, INPUT);
  Serial.begin(9600);
  Serial.print("starting up radio");
  radio.begin();
  Serial.println("...done");
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {
    buttonState = digitalRead(button);
    radio.write(&buttonState, sizeof(buttonState));
    delay(5);
}
