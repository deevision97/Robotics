
#include "HX711.h"
#include "TinyDHT.h"

#define calibration_factor -99990  //109525
#define DHTPIN 13  // DHT connected to Arduino Uno Digital Pin 2

#define DHTTYPE DHT11   // DHT 11 

#define LOADCELL_DOUT_PIN  6
#define LOADCELL_SCK_PIN  5

DHT dht(DHTPIN, DHTTYPE);

HX711 scale;
float weight = 0.0;

int data1, data2, data3, data4;
float data5, data6, data7, data8;
String data9, data10, data11, data12;


void setup() {
  Serial.begin(9600);
  //Serial.println("HX711 scale demo");

  dht.begin();

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(calibration_factor); //This value is obtained by using the Calibration sketch
  scale.tare(); //Assuming there is no weight on the scale at start up, reset the scale to 0
}

void loop() {
  //Serial.print("Reading: ");
  int8_t humidity = dht.readHumidity();
  int16_t temperature = dht.readTemperature(1);
  
  weight = scale.get_units(5);
  if(weight < 0){
  weight = (-1)*weight;
  }
  //weight = weight /2;

  data1 = humidity;
  data2 = temperature;
  data3 = weight;
  data4 = "RFID";

Serial.print(data1);      Serial.print("A");
Serial.print(data2);      Serial.print("B"); 
Serial.print(data3, 2);   Serial.print("C"); 
Serial.print(data4);      Serial.print("D"); 
  
Serial.print("\n");
  
  delay(500);
}
