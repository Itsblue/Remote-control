#ifndef Remote_Control_Receiver_H
#define Remote_Control_Receiver_H

#define RF24_CNS 7    // this is 7 for the Nano, D4 for the ESP
#define RF24_CE 8     // this is 8 for the ESP, D3 for the ESP

//--------------- define the structure and type of data that sender and receiver will exchange ----------------
typedef enum {HNDSHK, SERVO, MOTOR, LED, BUT, TEMP} data_type_e; 

typedef struct cmd_struct{
  data_type_e type;
  int int_data;
  float float_data;
  boolean boolean_data;
}cmd_s;

#endif
