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

const char HTML1[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
<style>
    html{font-family:Helvetica; text-align: center;}
    body {margin-top: 50px;}
    h1 {color:#444444; margin: 50px auto 30px;}
    h3 {color:#444444; margin-bottom: 50px auto 30px;}
    .gumb {background-color: #058105; display:block; width: 200px; color:yellow; padding: 13px 30px; text-decoration: none; 
    font-size: 25px; font-weight: bold; margin: 0px auto 35px; cursor: pointer; border-radius: 4px; border: none;}
    p {font-size: 16px; color: black; margin-bottom: 1px;}
</style>
    <title>Esp32</title>
</head>
<body>
    <h1> ESP32 spletni strežnik</h1>
    <h3>Priklop na WiFi usmerjevalnik</h3>
    <a class="gumb" href="/1">Prižgi LED</a>
    <a class="gumb" href="/0">Ugasni LED</a>
    <P>Prejet ukaz za vklop LED diode.</P>
</body>
</html>
)rawliteral";

const char HTML0[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
<style>
    html{font-family:Helvetica; text-align: center;}
    body {margin-top: 50px;}
    h1 {color:#444444; margin: 50px auto 30px;}
    h3 {color:#444444; margin-bottom: 50px auto 30px;}
    .gumb {background-color: #058105; display:block; width: 200px; color:yellow; padding: 13px 30px; text-decoration: none; 
    font-size: 25px; font-weight: bold; margin: 0px auto 35px; cursor: pointer; border-radius: 4px; border: none;}
    p {font-size: 16px; color: black; margin-bottom: 1px;}
</style>
    <title>Esp32</title>
</head>
<body>
    <h1> ESP32 spletni strežnik</h1>
    <h3>Priklop na WiFi usmerjevalnik</h3>
    <a class="gumb" href="/1">Prižgi LED</a>
    <a class="gumb" href="/0">Ugasni LED</a>
    <P>Prejet ukaz za izklop LED diode.</P>
</body>
</html>
)rawliteral";

String HTML3 ="<!DOCTYPE html>\n\
<html lang='sl'>\n\
<head>\n\
    <meta charset='UTF-8'>\n\
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>\n\
<style>\n\
    html{font-family:Helvetica; text-align: center;}\n\
    body {margin-top: 50px;}\n\
    h1 {color:#444444; margin: 50px auto 30px;}\n\
    h3 {color:#444444; margin-bottom: 50px auto 30px;}\n\
    .gumb {background-color: #058105; display:block; width: 200px; color:yellow; padding: 13px 30px; text-decoration: none; \n\
    font-size: 25px; font-weight: bold; margin: 0px auto 35px; cursor: pointer; border-radius: 4px; border: none;}\n\
    p {font-size: 16px; color: black; margin-bottom: 1px;}\n\
</style>\n\
    <title>Esp32</title>\n\
</head>\n\
<body>\n\
    <h1> ESP32 spletni strežnik</h1>\n\
    <h3>Priklop na WiFi usmerjevalnik</h3>\n\
    <a class='gumb' href='/1'>Prižgi LED</a>\n\
    <a class='gumb' href='/0'>Ugasni LED</a>\n\
    <P>HTTP strežnik je zagnan.</P>\n\
</body>\n\
</html>\n\ ";

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

  Serial.println("Vzpostavitev programske dostopne točke na esp32 modulu je uspela!");
  Serial.print("IP naslov esp32 dostopne točke je: ");
  Serial.println(WiFi.softAPIP()); 

  server.on("/", handle_root);
  server.on("/1", handle_1); // ko vpisemo http://192.168.1.195/1 -> sprozimo funkcijo handle_1
  server.on("/0", handle_0); //
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