#include "MQ135.h"
#include "MQ3.h"
#define MQ_PIN 13
MQ3 mq(MQ_PIN, false, 1000);
MQ135 mqco2(27);
#include "MQ2.h"
int pin = 35;
MQ2 mq2(pin);
#include <WiFi.h>
#include "ThingSpeak.h" 

const char* ssid = "Ravi's Galaxy F62";    
const char* password = "ravikola";   

WiFiClient  client;

unsigned long myChannelNumber = 1;
const char * myWriteAPIKey = "4WXQRJ28RG8IB89U";

unsigned long lastTime = 0;
unsigned long timerDelay = 30000;


void setup() {
 Serial.begin(115200);
 mq.begin();
 mq2.begin();
 WiFi.mode(WIFI_STA);   
 ThingSpeak.begin(client);  // Initialize ThingSpeak
 
}

void loop() {
    if ((millis() - lastTime) > timerDelay) {
    
    // Connect or reconnect to WiFi
    if(WiFi.status() != WL_CONNECTED){
      Serial.print("Attempting to connect");
      while(WiFi.status() != WL_CONNECTED){
        WiFi.begin(ssid, password); 
        delay(5000);     
      } 
      }
      Serial.println("\nConnected.");
    }
  float alcohol = mq.readRawValueOfAlcohol();
  Serial.print("alcohol:");
  Serial.println(mq.convertRawToPPM(alcohol), 0); Serial.print(F("ppm ; "));
  float* values = mq2.read(true); //true to print the values in the Serial
  float lpg = mq2.readLPG();
  float co = mq2.readCO();
  float smoke = mq2.readSmoke();
  Serial.print("lpg:");
  Serial.println(lpg);
  Serial.print("co:");
  Serial.println(co);
  Serial.print("smoke:");
  Serial.println(smoke);
  Serial.print("Co2:");
  Serial.println(mqco2.getPPM());
  int x = ThingSpeak.writeField(myChannelNumber, 1, mqco2.getPPM(), myWriteAPIKey);
  delay(100);
  
}
