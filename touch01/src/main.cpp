#include <Arduino.h>

//na nožico 27 je priključena žička-esp32 bo zaznal dotik
//ADC - analogno digitalni pretvornik, ki omogoča branje analognih signalov (npr. iz potenciometra) in njihovo pretvorbo v digitalne vrednosti, ki jih lahko obdeluje mikrokontroler
//12 bitne resolucije, kar pomeni, da lahko ADC pretvori analogni signal v digitalno vrednost med 0 in 4095 (2^12 vrednosti, to je 4096 različnih vrednosti, vključno z 0)
const int nozicaZaDotik=27; // nožica, na katero je priključena žička, ki bo zaznavala dotik (analogni vhod)

int vrednostDotika; // spremenljivka, ki bo hranila trenutno vrednost potenciometra


void setup() {
  // put your setup code here, to run once:
  // tu zapišemo kodo, ki se izvede le enkrat - nastavitve
  Serial.begin(115200);


  Serial.println("Testiranje delovanja zaznave dotika na nožici 27");

  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH); // na nožici 2 zapišemo visoko vrednost - LED zasveti
  delay(500);
  digitalWrite(2, LOW);
  delay(500); // zakasnitev izvedbe programa za specificiran čas v milisekundah, 1000 ms = 1 s
  digitalWrite(2, HIGH); // na nožici 2 zapišemo nizko vrednost - LED ugasne
  delay(500); // zakasnitev izvedbe programa za specificiran čas v milisekundah, 1000 ms = 1 s
  digitalWrite(2, LOW);
}

void loop() {
vrednostDotika=touchRead(nozicaZaDotik); // preberemo trenutno vrednost potenciometra in jo shranimo v spremenljivko
Serial.println(vrednostDotika);
delay(10); // zakasnitev izvedbe programa za specificiran čas v milisekundah, 1000 ms = 1 s
}