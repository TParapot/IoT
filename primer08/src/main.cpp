#include <Arduino.h>
#include "WiFi.h"
#include "WebServer.h"
#include <WiFiAP.h>

const char* ssid = "esp32_bajsi";
const char* password = "88888888";

WebServer server (80);
uint8_t LED1pin=2;
bool LED1status=LOW;
// vsebina HTML strani, ki se bo posredovala iz nasega esp32 spletnega streznika na 
// brskalnik klienta, tu lahko dodamo tudi CSS
// nasa spletna stran bo spravljena v spremenljivki HTML tipa string

String HTML1 = "<!DOCTYPE html>\n\
<html lang='sl'>\n\
<head>\n\
    <meta charset='UTF-8'>\n\
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>\n\
    <title>esp32</title>\n\
</head>\n\
<body>\n\
    <h1>Prejet ukaz za vklop LED diode</h1>\n\
</body>\n\
</html>\n\
";

String HTML0 = "<!DOCTYPE html>\n\
<html lang='sl'>\n\
<head>\n\
    <meta charset='UTF-8'>\n\
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>\n\
    <title>esp32</title>\n\
</head>\n\
<body>\n\
    <h1>Prejet ukaz za izklop LED diode</h1>\n\
</body>\n\
</html>\n\
";

String HTML3 = "<!DOCTYPE html>\n\
<html lang='sl'>\n\
<head>\n\
    <meta charset='UTF-8'>\n\
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>\n\
    <title>esp32</title>\n\
</head>\n\
<body>\n\
    <h1>Strežnik je zagnan. V brskalnik vpišete http://192.168.1.147/1 ali http://192.168.1.147/0, t.j. /1 ali /0 na koncu. Uporabimo http ne https (brez s)</h1>\n\
</body>\n\
</html>\n\
";
void handle_root(){
  server.send(200, "text/html;charset=utf-8", HTML3);
}

void handle_1() {
  LED1status=HIGH;
  Serial.println("GPI02 Status: ON");
  server.send(200, "text/html", HTML1); // ob zahtevi (request) "/1" bomo posredovali spletno stran ki je v string spremenljivki HTML1
} 

void handle_0() {
  LED1status=LOW;
  Serial.println("GPI02 Status: OFF");
  server.send(200, "text/html", HTML0);
}

void handle_NotFound() {
  LED1status=LOW;
  server.send(404, "text/html", "Spletna stran ni bila najdena");
}

void setup() {
  // put your setup code here, to run once:
  // tu zapišemo kodo, ki se izvede le enkrat - nastavitve
  Serial.begin(115200);

  WiFi.softAP(ssid, password);

//izpisemo IP naslov
  Serial.println("Vzpostavitev programske dostopne točke na esp32 modulu je uspela!");
  Serial.print("IP naslov esp32 dostopne točke je: ");
  Serial.println(WiFi.softAPIP()); 
  
  server.on("/", handle_root);
  server.on("/1", handle_1); // ko vpisemo http://192.168.1.195/1 -> sprozimo funkcijo handle_1
  server.on("/0", handle_0);
  server.onNotFound(handle_NotFound);
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

  if(LED1status) {
    digitalWrite(LED1pin, HIGH);
  } else {
    digitalWrite(LED1pin, LOW);
  }
} 