// Sensor de distância 

#include "Adafruit_VL53L0X.h"

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

// Definindo leds

// Carros
int ledVermelhoCarros = 33;
int ledAmareloCarros = 25;
int ledVerdeCarros = 26;

// Pedestres
int ledVermelhoPedestres = 27;
int ledVerdePedestres = 32;

// Definindo botão 
int buttonPin = 18;

void setup() {
  

}

void loop() {
  
}

// Para facilitar o código, apagando todos os leds e deixando apenas os necessários acesos!

void apagaLed(){
  digitalWrite(luzVermelhaCarros, LOW);
  digitalWrite(luzAmarelaCarros, LOW);
  digitalWrite(luzVerdeCarros, LOW);
  digitalWrite(luzVermelhaPedestres, LOW);
  digitalWrite(luzVerdePedestres, LOW);
}
