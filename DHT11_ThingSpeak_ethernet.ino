#include <DHT.h>
#include <ThingSpeak.h>
#include <SPI.h>
#include <Ethernet.h>

byte mac[] = {0x3C, 0xB7, 0x29, 0xDE, 0xFE, 0xED}; 
// Rimuovi l'IP statico e usa DHCP
EthernetClient client;
const char* api_key = "[Api Key]";
unsigned long myChannelNumber = [channel number];
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

void testInternet() {
  if (client.connect("api.thingspeak.com", 80)) {
    Serial.println("Connesso a ThingSpeak!");
    client.stop();
  } else {
    Serial.println("Connessione fallita!");
  }
}

void setup() {
  Serial.begin(115200);
  Ethernet.begin(mac); // Modalità DHCP
  delay(1000); // Attendi l'inizializzazione di Ethernet
  ThingSpeak.begin(client);
  dht.begin();
  
  Serial.print("IP: ");
  Serial.println(Ethernet.localIP());
  testInternet(); // Testa la connessione di rete
}

void loop() {
  if (Ethernet.maintain() != 0) { // Gestisci il rinnovo DHCP
    Serial.println("Errore connessione Ethernet");
    delay(1000);
    return;
  }

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  if (isnan(h) || isnan(t)) {
    Serial.println("Errore DHT");
    return;
  }

  ThingSpeak.setField(1, t);
  ThingSpeak.setField(2, h);
  
  int status = ThingSpeak.writeFields(myChannelNumber, api_key);
  if (status == 200) {
    Serial.println("Dati inviati!");
  } else {
    Serial.print("Errore: ");
    Serial.println(status); // Mostra il codice di errore
  }

  delay(30000); // Aumenta l'intervallo
}