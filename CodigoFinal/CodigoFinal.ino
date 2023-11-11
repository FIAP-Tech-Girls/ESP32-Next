#include "Adafruit_VL53L0X.h"

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

int carroContador;

// Definindo leds

// Carros
int luzVermelhaCarros = 33;
int luzAmarelaCarros = 25;
int luzVerdeCarros = 26;

// Pedestres
int luzVermelhaPedestres = 27;
int luzVerdePedestres = 32;

// Definindo botão 
int botaoPedestres = 18;

bool botaoPressionado = false; // Facilita código 

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

void apagaLed();

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

  // Sensor distância
  // wait until serial port opens for native USB devices
  while (! Serial) {
    Serial.print('.');
    delay(100);
  }
  
  Serial.println("Adafruit VL53L0X test");
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  // power 
  Serial.println(F("VL53L0X API Simple Ranging example\n\n")); 
}


void loop() {
  // Sensor de distância
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

    digitalWrite(luzAmarelaCarros, HIGH);

    delay(2000); // Delay de 2 segundos para a transição

    digitalWrite(luzAmarelaCarros, LOW); // Desliga a luz amarela dos carros

    digitalWrite(luzVermelhaCarros, HIGH); // Fecha semafóro de carros

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
