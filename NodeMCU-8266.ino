#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h> 
#include <SPI.h>
#include <MFRC522.h>


constexpr uint8_t RST_PIN = D3;     // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = D4;     // Configurable, see typical pin layout above
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;


String apiKey = "16BQBWQ6CVN0ML84";     //  Enter your Write API key from ThingSpeak

const char *ssid =  "Airtel_9827130206";     //Add WIFI Credentials
const char *pass =  "air70224";
const char* server = "api.thingspeak.com";

WiFiClient client;


#define BLYNK_TEMPLATE_ID "TMPLcBdcwazu"
#define BLYNK_DEVICE_NAME "Node MCU"
#define BLYNK_AUTH_TOKEN "2JPOA9VqoITM_1iLyVFZWdSh0CUU7_Ts"

#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>


SoftwareSerial mySerial (D1, D2); //rx tx

char auth[] = "2JPOA9VqoITM_1iLyVFZWdSh0CUU7_Ts";

char c; 
String DataIn; 
int8_t indexofA, indexofB, indexofC; //indexofD;
String data1, data2, data3, data4;

String tag;

 /* data1 - Humididty
  * data2- Temperature
  * data3- weight
  * data 4 - RFID
  */


void setup() {
  
  Serial.begin(57600);
  mySerial.begin(9600);
  
  Blynk.begin(auth, ssid, pass);

  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522

}


void loop() {
  Blynk.run();


   if ( ! rfid.PICC_IsNewCardPresent())
    return;
  if (rfid.PICC_ReadCardSerial()) {
    for (byte i = 0; i < 4; i++) {
      tag += rfid.uid.uidByte[i];
    }}


  data4 = tag;

  while(mySerial.available()>0){

        c = mySerial.read();


        

        if(c=='\n') {break;}
        else        {DataIn+=c;}
  }

  if(c=='\n'){

        Parse_the_Data();
        
          
     //serial monitor
          Serial.println("data1 = " + data1);
          Serial.println("data2 = " + data2);
          Serial.println("data3 = " + data3);
          Serial.println("data4 = " + data4);
          Serial.println("=================================");
          
          Blynk.virtualWrite(V1,data3);  
          Blynk.virtualWrite(V2,data1);
          Blynk.virtualWrite(V3,data2);
          Blynk.virtualWrite(V0,data4);

          /* V0- RFID
             V1 - weight
             v2 - Humididty
             V3 - Temperature*/


        if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
                      {  
                            
                             String postStr = apiKey;
                             postStr +="&field1=";
                             postStr += String(data3);
                             postStr +="&field2=";
                             postStr += String(data1);
                             postStr +="&field3=";
                             postStr += String(data2);
                             postStr +="&field4=";
                             postStr += String(data4);
                             postStr += "\r\n\r\n\r\n\r\n";

           /*field1 - weight
             field2 - Humidity
             field3 - Temperature
             field4 - RFID*/
 
                             client.print("POST /update HTTP/1.1\n");
                             client.print("Host: api.thingspeak.com\n");
                             client.print("Connection: close\n");
                             client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                             client.print("Content-Type: application/x-www-form-urlencoded\n");
                             client.print("Content-Length: ");
                             client.print(postStr.length());
                             client.print("\n\n");
                             client.print(postStr);

                        }
          client.stop(); 
          
      
        //reset
        c=0;
        DataIn="";
        
  }
  tag = "";
}
void Parse_the_Data()
{
  indexofA = DataIn.indexOf("A");
  indexofB = DataIn.indexOf("B");
  indexofC = DataIn.indexOf("C");
   
  data1  = DataIn.substring (0, indexofA);
  data2  = DataIn.substring (indexofA+1, indexofB);
  data3  = DataIn.substring (indexofB+1, indexofC);

}
