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
int botaoPedestres = 18;

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

bool botaoPressionado = false;

void setup() {
  Serial.begin(115200);
  
  // Botão
  pinMode(botaoPedestres,  INPUT_PULLUP);

  // Leds - Carros
  pinMode(luzVermelhaCarros, OUTPUT);
  pinMode(luzAmarelaCarros, OUTPUT);
  pinMode(luzVerdeCarros, OUTPUT);
  
  // Leds - Pedestres
  pinMode(luzVermelhaPedestres, OUTPUT);
  pinMode(luzVerdePedestres, OUTPUT);
}

void loop() {
  
  VL53L0X_RangingMeasurementData_t measure;
  
  Serial.print("Reading a measurement... ");
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
    Serial.print("Distance (mm): "); Serial.println(measure.RangeMilliMeter);
  } else {
    Serial.println(" out of range ");
  }
    
  delay(100);

  int distancia = measure.RangeMilliMeter;
    
  // Determinando o estado do semáforo com base na leitura da distância
  if (distancia >= 200) {
    carroContador = 0;
  } else if (distancia < 200 && distancia >= 20) {
    carroContador++;
  }    

  // Verificar se o botão de pedestres foi pressionado
  if (digitalRead(botaoPedestres) == LOW) {
    botaoPressionado = true;
  }

  // Lógica para o botão de pedestres pressionado
  if (botaoPressionado) {
    apagaLed(); // Desliga todas as luzes

    digitalWrite(luzVermelhaCarros, HIGH); // Fecha o semáforo dos carros
    digitalWrite(luzAmarelaCarros, HIGH);

    delay(2000); // Delay de 2 segundos para a transição

    digitalWrite(luzAmarelaCarros, LOW); // Desliga a luz amarela dos carros

    digitalWrite(luzVerdePedestres, HIGH); // Abre o semáforo dos pedestres

    delay(5000); // Tempo de abertura do semáforo dos pedestres (5 segundos neste exemplo)

    digitalWrite(luzVerdePedestres, LOW); // Fecha o semáforo dos pedestres

    botaoPressionado = false; // Reinicia a variável de botão pressionado para falso
  }

  // Controlar as mudanças de estado do semáforo dos carros
  unsigned long currentMillis = millis();
  switch (carState) {
    case GREEN:
      if (carroContador == 0) {
        carState = YELLOW;
        lastCarStateChange = currentMillis;
      }
      break;

    case YELLOW:
      if (currentMillis - lastCarStateChange >= 2000) {  // Tempo de espera da luz amarela: 2 segundos
        carState = RED;
        lastCarStateChange = currentMillis;
      }
      break;

    case RED:
      if (currentMillis - lastCarStateChange >= carStateDuration) {
        carState = GREEN;
        lastCarStateChange = currentMillis;
      }
      break;
  }

  // Controlar as luzes com base no estado atual do semáforo dos carros
  switch (carState) {
    case GREEN:
      apagaLed();
      digitalWrite(luzVerdeCarros, HIGH);
      digitalWrite(luzVermelhaPedestres, HIGH);
      break;

    case YELLOW:
      apagaLed();
      digitalWrite(luzAmarelaCarros, HIGH);
      break;

    case RED:
      apagaLed();
      digitalWrite(luzVermelhaCarros, HIGH);
      digitalWrite(luzVerdePedestres, HIGH);
      break;
  }
}

// Para facilitar o código, apagando todos os leds e deixando apenas os necessários acesos!
void apagaLed(){
  digitalWrite(luzVermelhaCarros, LOW);
  digitalWrite(luzAmarelaCarros, LOW);
  digitalWrite(luzVerdeCarros, LOW);
  digitalWrite(luzVermelhaPedestres, LOW);
  digitalWrite(luzVerdePedestres, LOW);
}
