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

// Defina os estados para controlar o semáforo dos carros
enum CarState {
  GREEN,
  YELLOW,
  RED
};

// Variáveis para controlar o estado do semáforo dos carros
CarState carState = RED;  // Comece com o estado vermelho
unsigned long lastCarStateChange = 0;
unsigned long carStateDuration = 10000;  // Tempo em milissegundos para cada estado dos carros

void setup() {
  Serial.begin(115200);
  
  // Botão
  pinMode(buttonPin,  INPUT_PULLUP);

  // Leds - Carros
  pinMode(luzVermelhaCarros, OUTPUT);
  pinMode(luzAmarelaCarros, OUTPUT);
  pinMode(luzVerdeCarros, OUTPUT);
  
  // Leds - Pedestres
  pinMode(luzVermelhaPedestres, OUTPUT);
  pinMode(luzVerdePedestres, OUTPUT);
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
