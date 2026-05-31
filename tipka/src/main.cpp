#include <Arduino.h>

int trenutnoStanje; // spremenljivka, ki bo hranila trenutno stanje tipke - ali je pritisnjena ali ne
const int BUTTON_PIN = 18; // nožica, na katero je priključena tipka

void setup() {
  // put your setup code here, to run once:
  // tu zapišemo kodo, ki se izvede le enkrat - nastavitve
  Serial.begin(115200);


  Serial.println("Testiranje delovanja tipke na nožici 18");

  pinMode(2, OUTPUT);
  pinMode(BUTTON_PIN, INPUT); // nastavitev nožice 18 kot vhod
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
trenutnoStanje = digitalRead(BUTTON_PIN); // preberemo stanje tipke na nožici 18 in ga shranimo v spremenljivko
if (trenutnoStanje == HIGH) { // če je tipka pritisnjena (HIGH), potem:
  Serial.println("Tipka je pritisnjena!"); // izpišemo sporočilo na serijski monitor
  digitalWrite(2, HIGH); // na nožici 2 zapišemo visoko vrednost - LED zasveti
} else { // če tipka ni pritisnjena (LOW), potem:
  Serial.println("Tipka ni pritisnjena!"); // izpišemo sporočilo na serijski monitor
  digitalWrite(2, LOW); // na nožici 2 zapišemo nizko vrednost - LED ugasne
}
}