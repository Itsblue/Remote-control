#ifndef Remote_Control_Receiver_H
#define Remote_Control_Receiver_H

#define RF24_CNS 7    // this is 7 for the ESP
#define RF24_CE 8     // this is 8 for the ESP

//--------------- define the structure and type of data that sender and receiver will exchange ----------------
typedef enum {HANDSHAKE, TRANS, SERVO, MOTOR, LED, BUT, TEMP} data_typ_e; 
typedef enum {UPDATE,NOUPDATE} update_e;
typedef enum {DATA, HNDSHK, COMMAND, REQUEST} send_rec_e;

typedef struct but_struct{
  send_rec_e cmd_req;
  update_e upd;
  boolean state;
}but_s;



typedef struct trans_data_struct
{
  send_rec_e trans;      //type of data, command
  byte hndshk_sig;        //handshake value
  but_s but;
} trans_data_s ;//= {HNDSHK, 0,{COMMAND,NOUPDATE,LOW}};
#endif
