#include <Arduino.h>
#include "WiFi.h"
#include "WebServer.h"
#include <WiFiAP.h> //dodali smo Access point

const char* ssid = "esp32_bajsi";
const char* password = "88888888";

WebServer server (80);

// vsebina html strani,ki se posredovala iz nasega esp32 spletnega streznika na brskalnik klienta, 
//tu lahko dodamo tudi CSS nasa spletna stran
//nasa spletna stran bo spravljena v spremenljivki HTML tipa String
String HTML="<!DOCTYPE html>\n\
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
  server.send(200, "text/html;charset=utf-8", HTML);
}
void setup() {
  // put your setup code here, to run once:
  // tu zapišemo kodo, ki se izvede le enkrat - nastavitve
  Serial.begin(115200);


  //vzpostavitev programske dostopne točke - soft access point AP
  //software enabled access point, racunalnik, ki ni bil zasnovan kot dostopna tocka postane z ustreznim programjem dostopna tocka
  //Esp32 se spremeni v dostopno tocko
  WiFi.softAP(ssid, password);

//izpisemo IP naslov
  Serial.println("Vzpostavitev programske dostopne točke na esp32 modulu je uspela!");
  Serial.print("IP naslov esp32 dostopne točke je: ");
  Serial.println(WiFi.softAPIP()); // prednastavljeni IP naslov dostopne točke je 192.168.4.1

  server.on("/", handle_root);
  server.begin();
  Serial.print("HTTP strežnik je zagnan, vpišite IP naslov v brskalnik.");
  Serial.println(WiFi.softAPIP());
  
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