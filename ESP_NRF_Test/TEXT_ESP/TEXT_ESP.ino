
#include <ESP8266WiFi.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define led D1



RF24 radio(D4, D8); // CNS, CE

const byte address[6] = "00001";
boolean buttonState = 0;


void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();
  Serial.println("hello");
  pinMode(led, OUTPUT);

}

void loop() {

  radio.startListening();
  while (!radio.available());
  radio.read(&buttonState, sizeof(buttonState));
  if (buttonState == HIGH) {
    digitalWrite(led, HIGH);
    Serial.println("HIGH");
  }
  else {
    digitalWrite(led, LOW);
    Serial.println("LOW");
  }
}
