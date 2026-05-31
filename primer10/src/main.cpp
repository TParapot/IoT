#include <Arduino.h>
#include "WiFi.h"
#include "WebServer.h"
#include "WebSocketsServer.h" //knjiznjica za spletne vticnike

const char* ssid = "kresilnik";
const char* password = "bled2024";

WebServer server (80);

// na vratih 81 ustvarimo streznik spletnih vticnikov web sockets serverjev

WebSocketsServer webSocket = WebSocketsServer(81);

uint8_t LED1pin = 2; //8 bitna celostevilska spremenljivka
bool LED1status = LOW; //bool spremenljivka za stanje na pinu


// vsebina HTML strani, ki se bo posredovala iz nasega esp32 spletnega streznika na 
// brskalnik klienta, tu lahko dodamo tudi CSS
// nasa spletna stran bo spravljena v spremenljivki HTML tipa string
String HTML0 = "<!DOCTYPE html>\n\
<html lang='sl'>\n\
<head>\n\
    <meta charset='UTF-8'>\n\
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>\n\
    <title>esp32</title>\n\
</head>\n\
<body>\n\
    <h1>Primer s spletnim vticnikom WebSocket na portu 81.</h1>\n\
</body>\n\
</html>\n\
";

void handle_root(){
  server.send(200, "text/html;charset=utf-8", HTML0);
}

void handle_NotFound() {
  server.send(200, "text/html", "Spletna stran ni bila najdena."); //klientu sporocimo da zahtevanje spletne strani nismo nasli na nasem strezniku
}

//***********************************************
//definicija funkcije websocket event, ki se izvede vedno ko dobimo sporocilo preko spletnega vticnika
//***********************************************

void onWebSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  //dolocimo funkcionalnost glede na tip dogodka na spletnem vticniku

  switch(type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Klient odklopljen! \n", num);
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Klient povezan iz IP naslova ", num);
        Serial.println(ip.toString());
      }
      break;

      case WStype_TEXT:
      {
        Serial.printf("[%u] Prejeto sporocilo: %s \n", num, payload); //prejeto sporocilo iz spletnega vticnika izpisemo na serijski monitor, da smo sprejeli sporocilo
        String znak = String((char) payload[0]); //prejeto sporocilo pretvorimo v string, da ga lahko enostavno primerjamo z besedilom

        Serial.println(znak); //izpis sporocila na serijski monitor

        if (znak == "1") 
        { //ce je prejeto sporocilo "1" potem vklopimo LED
          digitalWrite(LED1pin, HIGH); //na nožici LED1pin zapišemo visoko vrednost - LED zasveti
        }
        else if (znak == "0") 
        { //ce je prejeto sporocilo "0" potem izklopimo LED
          digitalWrite(LED1pin, LOW); //na nožici LED1pin zapišemo nizko vrednost - LED ugasne
        }
      }

    default:
      break;
    }
}

void setup() {
  // put your setup code here, to run once:
  // tu zapišemo kodo, ki se izvede le enkrat - nastavitve
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
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
  server.onNotFound(handle_NotFound); // v primeru da spletne strani ni na strezniku
  server.begin();

  //pozenemo web socket streznik

  webSocket.begin();
  webSocket.onEvent(onWebSocketEvent);

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

  webSocket.loop();
}