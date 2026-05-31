#include <Arduino.h>
#include "WiFi.h"
#include "WebServer.h"
#include "WebSocketsClient.h" //knjiznjica za spletne vticnike
#include <ArduinoJson.h> //knjiznjica za delo z JSON objekti 

const char* ssid = "kresilnik";
const char* password = "bled2024";

int portNumber = 8811; // port na katerem bo deloval spletni streznik, lahko ga poljubno spremenimo, vendar pazimo da ni v uporabi za druge namene

const int nozicaPotenciometra=34; // nožica, na katero je priključen potenciometer

int vrednostPotenciometra; // spremenljivka, ki bo hranila trenutno vrednost potenciometra

String dataString; // spremenljivka, ki bo hranila JSON sporočilo, ki ga bomo poslali preko spletnega vtičnika 8811 na node.js strežnik

WebSocketsClient webSocket; //ustvarimo objekt webSocket, ki bo predstavljal spletni vticnik

void setup() {
  // put your setup code here, to run once:
  // tu zapišemo kodo, ki se izvede le enkrat - nastavitve
  Serial.begin(115200);

  WiFi.begin(ssid,password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Povezovanje z WiFi omrežjem...");
  }

  Serial.println("Povezava z WiFi omrežjem je vzpostavljena");
  Serial.print("IP naslov esp32 modula je: ");
  Serial.println(WiFi.localIP());

  webSocket.begin("192.168.1.163", 8811); // povezava na node.js strežnik, ki teče na IP naslovu 192.168.1.136 preko vrat 8811
  

  pinMode(2, OUTPUT);
  pinMode(4,OUTPUT);
  digitalWrite(2, HIGH); // na nožici 2 zapišemo visoko vrednost - LED zasveti
  delay(500);
  digitalWrite(2, LOW);
  delay(500); // zakasnitev izvedbe programa za specificiran čas v milisekundah, 1000 ms = 1 s
  digitalWrite(2, HIGH); // na nožici 2 zapišemo nizko vrednost - LED ugasne
  delay(500); // zakasnitev izvedbe programa za specificiran čas v milisekundah, 1000 ms = 1 s
  digitalWrite(2, LOW);

  Serial.println(WiFi.localIP());
  Serial.println(WiFi.getMode());

}


void loop() {
  webSocket.loop();
  vrednostPotenciometra=analogRead(nozicaPotenciometra); // preberemo trenutno vrednost potenciometra in jo shranimo v spremenljivko
  Serial.println(vrednostPotenciometra); // izpišemo trenutno vrednost potenciometra na serijski monitor
  
  dataString = R"({"tipSporočila":"potenciometer","pin":34,"vrednost":)";
  dataString = dataString + String (vrednostPotenciometra) + "}"; // sestavimo JSON sporočilo, ki ga bomo poslali preko spletnega vtičnika 8811 na node.js strežnik, v sporočilu je tipSporočila "potenciometer", pin 34 in vrednost trenutne vrednosti potenciometra
  
  webSocket.sendTXT(dataString); // pošljemo JSON sporočilo preko spletnega vtičnika na portu 8811 na node.js strežnik

  Serial.println(dataString); // izpišemo JSON sporočilo na serijski monitor

  delay (100); // zakasnitev izvedbe programa za specificiran čas v milisekundah, 1000 ms = 1 s
}