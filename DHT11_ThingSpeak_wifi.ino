#include <DHT.h>
#include <ThingSpeak.h>
#include <WiFiS3.h>

const char * api_key = "[Api Key]";
const char * ssid = "[Nome WiFi]";
const char * pass = "[Password WiFi]";
unsigned long myChannelNumber = [channel number];
const char * server = "api.thingspeak.com";

#define DHTPIN 8
#define DHTTYPE DHT11
DHT dht (DHTPIN, DHTTYPE);

WiFiClient client;
float temp;
float hum;

void setup(){

  ThingSpeak.begin(client);
  dht.begin();
  WiFi.begin(ssid, pass);
  
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

}
void loop(){

  temp = dht.readTemperature();
  hum = dht.readHumidity();

  ThingSpeak.setField(1, temp);
  ThingSpeak.setField(2, hum);
  ThingSpeak.writeFields(myChannelNumber, api_key);

  delay(15000);

}
