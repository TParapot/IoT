#include <Arduino.h>

//na nožico 34 je priključena srednja nožica potenciometra
//ADC - analogno digitalni pretvornik, ki omogoča branje analognih signalov (npr. iz potenciometra) in njihovo pretvorbo v digitalne vrednosti, ki jih lahko obdeluje mikrokontroler
//12 bitne resolucije, kar pomeni, da lahko ADC pretvori analogni signal v digitalno vrednost med 0 in 4095 (2^12 vrednosti, to je 4096 različnih vrednosti, vključno z 0)
const int nozicaPotenciometra=34; // nožica, na katero je priključen potenciometer

int vrednostPotenciometra; // spremenljivka, ki bo hranila trenutno vrednost potenciometra


void setup() {
  // put your setup code here, to run once:
  // tu zapišemo kodo, ki se izvede le enkrat - nastavitve
  Serial.begin(115200);


  Serial.println("Testiranje delovanja potenciometra na nožici 34");

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
vrednostPotenciometra=analogRead(nozicaPotenciometra); // preberemo trenutno vrednost potenciometra in jo shranimo v spremenljivko
Serial.println(vrednostPotenciometra);
delay(100); // zakasnitev izvedbe programa za specificiran čas v milisekundah, 1000 ms = 1 s
}