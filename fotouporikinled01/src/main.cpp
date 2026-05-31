#include <Arduino.h>
#include "WiFi.h"
#include "WebServer.h"
#include "WebSocketsClient.h"
#include <ArduinoJson.h>

const char* ssid = "kresilnik";
const char* password = "bled2024";

int portNumber = 8811;

const int nozicaFotoupornika = 32;

int vrednostFotoupornika;
String dataString;

WebSocketsClient webSocket;

// PROTOTIP FUNKCIJE
void webSocketEvent(WStype_t type, uint8_t * payload, size_t length);

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Povezovanje z WiFi omrežjem...");
  }

  Serial.println("Povezava z WiFi omrežjem je vzpostavljena");
  Serial.print("IP naslov esp32 modula je: ");
  Serial.println(WiFi.localIP());

  webSocket.begin("192.168.1.11", 8811);
  webSocket.onEvent(webSocketEvent);

  pinMode(2, OUTPUT);
  pinMode(13, OUTPUT);

  // LED na pin 13
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  digitalWrite(2, HIGH);
  delay(500);
  digitalWrite(2, LOW);
  delay(500);
  digitalWrite(2, HIGH);
  delay(500);
  digitalWrite(2, LOW);

  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW);
  delay(500);
  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW);

  Serial.println(WiFi.localIP());
  Serial.println(WiFi.getMode());
}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {

    case WStype_TEXT:
    {
      StaticJsonDocument<256> doc;

      DeserializationError error = deserializeJson(doc, payload, length);

      if (error) {
        Serial.print("  Napaka pri deserializaciji JSON sporočila: ");
        Serial.println(error.c_str());
        return;
      }

      char tip[40];
      memset(tip, 0, sizeof(tip));
      strcpy(tip, doc["tipSporočila"]);

      if (strcmp(tip, "LED") == 0) {

        int vrednost = doc["vrednost"].as<int>();
        int pin = doc["pin"].as<int>();

        // LED na pin 2
        if (pin == 2) {
          digitalWrite(2, vrednost == 1 ? HIGH : LOW);
        }


        if (pin == 13) {
          digitalWrite(13, vrednost == 1 ? HIGH : LOW);
        }
        // --- DODAN IF STAVEK ZA PIN 13 ---
      
        // ---------------------------------
      }

      Serial.print("Prejeto sporočilo preko spletnega vtičnika: ");
      Serial.println((char*)payload);
      break;
    }

    default:
      break;
  }
}

void loop() {
  webSocket.loop();

  vrednostFotoupornika = analogRead(nozicaFotoupornika);
  Serial.println(vrednostFotoupornika);

  dataString = R"({"tipSporočila":"fotoupornik","pin":32,"vrednost":)";
  dataString = dataString + String(vrednostFotoupornika) + "}";

  webSocket.sendTXT(dataString);

  delay(500);
}