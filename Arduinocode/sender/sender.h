#ifndef Remote_Control_Sender_H
#define Remote_Control_Sender_H

#define BUTTON 4

#define RF24_CNS 7    // this is 7 for the Arduino Nano
#define RF24_CE 8     // this is 8 for the Arduino Nano

//--------------- define the structure and type of data that sender and receiver will exchange ----------------
typedef enum {HANDSHAKE, TRANS, SERVO, MOTOR, LED, BUT, TEMP} data_typ_e; 
typedef enum {UPDATE,NOUPDATE} update_e;
typedef enum {HNDSHK, COMMAND, REQUEST} send_rec_e;

typedef struct but_struct{
  update_e upd;
  boolean state;
}but_s;

struct trans_data_struct
{
  send_rec_e trans;      //type of data, command
  int hndshk_sig;             //handshake value
  but_s but;
} trans_data_struct_e = {HNDSHK, 0,{.upd=NOUPDATE,.state=LOW}};
#endif
