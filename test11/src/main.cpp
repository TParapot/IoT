#include <Arduino.h>
#include "WiFi.h"
#include "WebServer.h"
#include "WebSocketsClient.h" //knjiznjica za spletne vticnike
#include <ArduinoJson.h> //knjiznjica za delo z JSON objekti 

const char* ssid = "kresilnik";
const char* password = "bled2024";

int portNumber = 8811; // port na katerem bo deloval spletni streznik, lahko ga poljubno spremenimo, vendar pazimo da ni v uporabi za druge namene

bool trenutnoStanje; // spremenljivka, ki bo hranila trenutno stanje tipke - ali je pritisnjena ali ne
const int BUTTON_PIN = 18; // nožica, na katero je priključena tipka

String dataString; // spremenljivka, ki bo hranila JSON sporočilo, ki ga bomo poslali preko spletnega vtičnika 8811 na node.js strežnik

WebSocketsClient webSocket; //ustvarimo objekt webSocket, ki bo predstavljal spletni vticnik


//koda, ki se izvede ob prejemu sporočila preko spletnega vtičnika (WebSocket-a) 8811
void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {

    case WStype_TEXT:
    {
      //v tem delu prenešeni string z JSON sporočilom pretvorimo v JSON objekt, da lahko z njim lažje delamo
      //<256> določa kapaciteto objekta doc - StaticJson document, ki je dovolj velika, da lahko vanj shranimo JSON sporočilo, ki ga prejmemo preko spletnega vtičnika
      StaticJsonDocument<256> doc;

      //deserializacija JSON sporočila, ki ga prejmemo preko spletnega vtičnika, in shranjevanje v objekt doc
      DeserializationError error = deserializeJson(doc, payload, length);

      //preverimo, če je prišlo do napake pri deserializaciji JSON sporočila
      if (error) {
        Serial.print("  Napaka pri deserializaciji JSON sporočila: ");
        Serial.println(error.c_str());
        return;
      }

      char tip[40]; // spremenljivka za besedo iz JSON strukture
      memset(tip, 0, sizeof(tip)); // inicializacija spremenljivke tip z ničlami
      strcpy(tip, doc["tipSporočila"]); // kopiranje vrednosti iz JSON objekta doc, ki je shranjena pod ključem "tip", v spremenljivko tip

      if (strcmp(tip, "LED") == 0) { // primerjava spremenljivke tip z besedo "LED", da preverimo ali je tip sporočila "LED"
        int vrednost = doc["vrednost"]; // pridobivanje vrednosti iz JSON objekta doc, ki je shranjena pod ključem "vrednost", in shranjevanje v spremenljivko vrednost
        int pin = doc["pin"]; // pridobivanje vrednosti iz JSON objekta doc, ki je shranjena pod ključem "pin", in shranjevanje v spremenljivko pin

        if (pin == 2) { // popravljeno iz pin=2
          if (vrednost == 0) { // popravljeno iz vrednost=0
            digitalWrite(2, LOW); // če je vrednost 0, potem na nožici 2 zapišemo visoko vrednost - LED ugasne
          } else if (vrednost == 1) { // popravljeno iz vrednost=1
            digitalWrite(2, HIGH); // če je vrednost 1, potem na nožici 2 zapišemo nizko vrednost - LED zasveti
          }
        }
      }

      Serial.print("Prejeto sporočilo preko spletnega vtičnika: ");
      Serial.println((char*)payload); // izpis prejetega sporočila preko spletnega vtičnika na serijski monitor
      break;
    }

    default:
      break;
  }
}


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

  webSocket.begin("192.168.1.136", 8811); // povezava na node.js strežnik, ki teče na IP naslovu 192.168.1.136 preko vrat 8811
  

  pinMode(2, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  pinMode(4,OUTPUT);
  digitalWrite(2, HIGH); // na nožici 2 zapišemo visoko vrednost - LED zasveti
  delay(500);
  digitalWrite(4, LOW);
  delay(500); // zakasnitev izvedbe programa za specificiran čas v milisekundah, 1000 ms = 1 s
  digitalWrite(2, HIGH); // na nožici 2 zapišemo nizko vrednost - LED ugasne
  delay(500); // zakasnitev izvedbe programa za specificiran čas v milisekundah, 1000 ms = 1 s
  digitalWrite(4, LOW);

  Serial.println(WiFi.localIP());
  Serial.println(WiFi.getMode());
  webSocket.onEvent(webSocketEvent);  // nastavimo funkcijo webSocketEvent, ki se bo izvedla ob prejemu sporočila preko spletnega vtičnika

}


void loop() {
  webSocket.loop();
  trenutnoStanje = digitalRead(BUTTON_PIN); // preberemo stanje tipke na nožici 18 in ga shranimo v spremenljivko
  Serial.println(trenutnoStanje); // izpis trenutnega stanja tipke na nožici 18 na serijski monitor

if (trenutnoStanje == HIGH) { // če je tipka pritisnjena (HIGH), potem:
  dataString = R"({"tipSporočila":"TIPKA","pin":18,"vrednost":1})"; //  R"()" - raw string literal, ki nam omogoča enostaven vnos
  webSocket.sendTXT(dataString); // pošljemo JSON sporočilo preko spletnega vtičnika
} else { // če tipka ni pritisnjena (LOW), potem:
  dataString = R"({"tipSporočila":"TIPKA","pin":18,"vrednost":0})"; //  R"()" - raw string literal, ki nam omogoča enostaven vnos
  webSocket.sendTXT(dataString); // pošljemo JSON sporočilo preko spletnega vtičnika
}

delay (4); // zakasnitev izvedbe programa za specificiran čas v milisekundah, 1000 ms = 1 s
}