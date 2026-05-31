#include <Arduino.h>

void setup() {
  // put your setup code here, to run once:
  // tu zapišemo kodo, ki se izvede le enkrat - nastavitve
  pinMode(2, OUTPUT);
  pinMode(4,OUTPUT); // nožica št. 2 bo delovala kot izhod
}

void loop() {
  // put your main code here, to run repeatedly:
  // tu zapišemo kodo, ki se ponavljajoče izvaja
  digitalWrite(2, HIGH); // na nožici 2 zapišemo visoko vrednost - LED zasveti
  delay(1500);
  digitalWrite(4, HIGH);
  delay(750); // zakasnitev izvedbe programa za specificiran čas v milisekundah, 1000 ms = 1 s
  digitalWrite(2, LOW); // na nožici 2 zapišemo nizko vrednost - LED ugasne
  digitalWrite(4, LOW);
  delay(250); // zakasnitev izvedbe programa za specificiran čas v milisekundah, 1000 ms = 1 s
}