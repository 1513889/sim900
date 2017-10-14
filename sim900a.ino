#include "SIM900.h" 
#include <SoftwareSerial.h> 
#include "sms.h"
#include "call.h"
CallGSM call_classname;
SMSGSM sms_classname;


char position;
char fra_get[30];
//char fra_start[2];
//char fra_type[1];
//char fra_func[3]; 
//char fra_lentgh[2];
char fra_data[20];
int lentgh=0;

 
boolean started=false; //trạng thái modul sim 
char smstext[160];// nội dung tin nhắn 
char number[20]; // số điện thoại format theo định dạng quốc tế
int i = 0;
unsigned long j=0;


void setup(){   
    Serial.begin(9600);   
    Serial.println("test sim900a");  
    if (gsm.begin(2400)){     
        Serial.println("\nstatus=READY");     
        started=true;     
    }   else 
        Serial.println("\nstatus=IDLE");  
    
    if(started){     
       // sms.SendSMS("+841656487085", "Online");   //đổi lại số của bạn nhé :D
    } 
}
 
void loop() {   
    if(started){     
//        Serial.print("\nbat dau nhan frame: ");
        //nhan frame        
        do {
          Serial.print(0x00ff21043030); //err
          while(Serial.available() == 0)  {
            j++;
            if (j>999999)  j=0;
          }          
          while(Serial.available()) {
            fra_get[i] = Serial.read();
//            Serial.print(fra_get[i]);    
            i++;
            delay(50);
          }
          i=0;
        }
        while((fra_get[0] != 0x00)|(fra_get[1] != 0xff));
        
        //lay data ra
        lentgh = (fra_get[4]-48)*10 + (fra_get[5]-48);
        for(i=0; i<lentgh; i++){
          fra_data[i] = fra_get[i+6];
        }
        i=0;
        //type
        switch(fra_get[2]) {
          case 0x20: {               //lenh
            if (fra_get[3]==0x01) {
              Serial.print(0x00ff21033030); //fr_1_okk_00
              call_classname.Call(fra_data);                  
            }
            else if (fra_get[3]==0x02) {
              smstext[0] = j/100000+48;
              delay(50);
              smstext[1] = (j-(smstext[0]-48)*100000)/10000+48;
              smstext[2] = (j-(smstext[0]-48)*100000-(smstext[1]-48)*10000)/1000+48;
              smstext[3] = (j-(smstext[0]-48)*100000-(smstext[1]-48)*10000-(smstext[2]-48)*1000)/100+48;
              smstext[4] = (j-(smstext[0]-48)*100000-(smstext[1]-48)*10000-(smstext[2]-48)*1000-(smstext[3]-48)*100)/10+48;
              smstext[5] = j-(smstext[0]-48)*100000-(smstext[1]-48)*10000-(smstext[2]-48)*1000-(smstext[3]-48)*100-(smstext[4]-48)*10+48;
              Serial.print(0x00ff21033030); //fr_1_okk_06
              Serial.print(smstext);
              Serial.println();
              sms_classname.SendSMS(fra_data, smstext);
            }
          }
          case 0x21: {               //ref
            if (fra_get[3]==0x03) {
              Serial.print(0x00ff21033030); //fr_1_okk_00
            }
            else if (fra_get[3]==0x04) {
              if (fra_get[3]==0x01) {
              Serial.print(0x00ff21033030); //fr_1_okk_00
              call_classname.Call(number);                  
              }
              else if (fra_get[3]==0x02) {
                smstext[0] = j/100000+48;
                smstext[1] = (j-(smstext[0]-48)*100000)/10000+48;
                smstext[2] = (j-(smstext[0]-48)*100000-(smstext[1]-48)*10000)/1000+48;
                smstext[3] = (j-(smstext[0]-48)*100000-(smstext[1]-48)*10000-(smstext[2]-48)*1000)/100+48;
                smstext[4] = (j-(smstext[0]-48)*100000-(smstext[1]-48)*10000-(smstext[2]-48)*1000-(smstext[3]-48)*100)/10+48;
                smstext[5] = j-(smstext[0]-48)*100000-(smstext[1]-48)*10000-(smstext[2]-48)*1000-(smstext[3]-48)*100-(smstext[4]-48)*10+48;
                Serial.print(0x00ff2103); //fr_1_okk_06
                Serial.print(smstext);
                sms_classname.SendSMS(fra_data, smstext);
              }
            }
          }
        }
    } else Serial.println("Offline");

}
