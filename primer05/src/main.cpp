#include <Arduino.h>
#include "WiFi.h"
#include "WebServer.h"

const char* ssid = "kresilnik";
const char* password = "bled2024";

WebServer server (80);

// vsebina html strani,ki se posredovala iz nasega esp32 spletnega streznika na brskalnik klienta, 
//tu lahko dodamo tudi CSS nasa spletna stran
//nasa spletna stran bo spravljena v spremenljivki HTML tipa String
String HTML1="<!DOCTYPE html>\n\
<html lang='sl'>\n\
<head>\n\
    <meta charset='UTF-8'>\n\
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>\n\
    <title>esp32</title>\n\
</head>\n\
<body>\n\
    <h1>Pozdravljen svet 111111111111! </h1>\n\
</body>\n\
</html>";

String HTML3="<!DOCTYPE html>\n\
<html lang='sl'>\n\
<head>\n\
    <meta charset='UTF-8'>\n\
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>\n\
    <title>esp32</title>\n\
</head>\n\
<body>\n\
    <h1>Pozdravljen svet iz esp32, čšđćž delujejo!</h1>\n\
</body>\n\
</html>";

void handle_root(){
  server.send(200, "text/html;charset=utf-8", HTML3);
}
void handle_1(){
  server.send(200,"text/htmL", HTML1); //OB ZAHTEVI "/1" bomo posredovali spletno stran
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

  server.on("/", handle_root);
  server.on("/1", handle_1);
  server.begin();

  pinMode(2, OUTPUT);
  pinMode(4,OUTPUT);
  digitalWrite(2, HIGH); // na nožici 2 zapišemo visoko vrednost - LED zasveti
  delay(500);
  digitalWrite(4, LOW);
  delay(500); // zakasnitev izvedbe programa za specificiran čas v milisekundah, 1000 ms = 1 s
  digitalWrite(2, HIGH); // na nožici 2 zapišemo nizko vrednost - LED ugasne
  delay(500); // zakasnitev izvedbe programa za specificiran čas v milisekundah, 1000 ms = 1 s
  digitalWrite(4, LOW);
}


void loop() {
  server.handleClient();
}