
#ifndef Remote_Control_transceiver_H
#define Remote_Control_transceiver_H

//-------------- defines for the OneWire teperature sensor 18B20 ---------------------------------------------
#define  ONE_WIRE_BUS 2 // Data wire is plugged into pin 2 on the Arduino Nano


//-------------- defines fpr the radio devices NRF24 ---------------------------------------------------------

#define RADIO_SEL 4   // this 4 for Nano
typedef enum {RADIO0 = 0, RADIO1} radio_type_e;      
#define RF24_CNS 7    // this is 7 for the Nano, D4 for the ESP
#define RF24_CE 8     // this is 8 for the ESP, D3 for the ESP

//--------------- define the structure and type of data that sender and receiver will exchange ----------------
typedef enum {HNDSHK, DATA, ALARM} data_type_e; 

typedef struct transcv_struct{
  data_type_e type;
  unsigned long hndshk; // This is to make sure we have a connection with the right client
  unsigned long time;
  int valserv;          // Expected range 0 - 180 degrees
  int valmot;           // Expected range 0 - 180 degrees
  float temperature;    // temperature messured by the client
  int counter;
}transcv_s;

#endif
