#include <SoftwareSerial.h>
#include "DHT.h"

#define DHTPIN 2     // Digital pin connected to the DHT sensor

#define DHTTYPE DHT22   // DHT 11

DHT dht(DHTPIN, DHTTYPE);
#define SensorPin A0
unsigned long int avgValue;
float b;
int buf[10],temp;
const byte nitro[] ={0x01, 0x03, 0x00, 0x1E, 0x00, 0x01, 0xE4, 0x0c};
const byte phos[] ={0x01, 0x03, 0x00, 0x1F, 0x00, 0x01, 0xB5, 0xcc};
const byte pota[] ={0x01, 0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xc0};
byte values[8];
SoftwareSerial mod(9,10);
void setup() {
  Serial.begin(4800);
  mod.begin(4800);
  dht.begin();
  delay(500);
}
void loop() {
byte val1=0,val2=0,val3=0;
float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);
for(int i=0;i<10;i++){
    buf[i]= analogRead(SensorPin);
    delay(10);
  }
  for(int i=0;i<9;i++){
    for(int j=i+1;j<10;j++){
      if(buf[i]>buf[j]){
        temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
  avgValue=0;
  for(int i=2;i<8;i++)
    avgValue=buf[i];
  float phValue=(float)avgValue*5.0/1024/6; 
  phValue = 3.5*phValue;
  val1 = nitrogen();
  delay(1500);
  val2 = phosphorous();
  delay(1500);
  val3 = potassium();
  delay(1500);
  Serial.print("");
  Serial.print(val1);
  Serial.print(",");
  Serial.print(val2);
  Serial.print(",");
  Serial.print(val3);
  Serial.print(",");
  Serial.print(phValue,2);
   Serial.print(",");
  Serial.print(h);
  Serial.print(",");
  Serial.println(t);
  delay(1000);
}
byte nitrogen(){
  mod.write(nitro,8);
  delay(100);
  for(byte i=0;i<7;i++){
    values[i] = mod.read();
  }
  return values[4];
}
byte phosphorous(){
  mod.write(phos,8);
  delay(100);
  for(byte i=0;i<7;i++){
    values[i] = mod.read();
  }
  return values[4];
}
byte potassium(){
  mod.write(pota,8);
  delay(100);
  for(byte i=0;i<7;i++){
    values[i] = mod.read();
  }
  return values[4];
}
