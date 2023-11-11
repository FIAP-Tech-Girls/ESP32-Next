//Autor: Fábio Henrique Cabrini
//Resumo: Esse programa possibilita ligar e desligar o led onboard, além de mandar o status para o Broker MQTT possibilitando o Helix saber
//se o led está ligado ou desligado.
//Revisões:
//Rev1: 26-08-2023 Código portado para o ESP32 e para realizar a leitura de luminosidade e publicar o valor em um tópico aprorpiado do broker 
//Autor Rev1: Lucas Demetrius Augusto 
//Rev2: 28-08-2023 Ajustes para o funcionamento no FIWARE Descomplicado
//Autor Rev2: Fábio Henrique Cabrini

#include <WiFi.h>
#include <PubSubClient.h> // Importa a Biblioteca PubSubClient
#include "Adafruit_VL53L0X.h"



//defines:
//defines de id mqtt e tópicos para publicação e subscribe denominado TEF(Telemetria e Monitoramento de Equipamentos)
#define TOPICO_SUBSCRIBE    "/TEF/tts300/cmd"        //tópico MQTT de escuta
#define TOPICO_PUBLISH      "/TEF/tts300/attrs"      //tópico MQTT de envio de informações para Broker
#define TOPICO_PUBLISH_2    "/TEF/tts300/attrs/c"    //tópico MQTT de envio de informações para Broker
#define TOPICO_PUBLISH_3    "/TEF/tts300/attrs/s1"                                                      //IMPORTANTE: recomendamos fortemente alterar os nomes
#define TOPICO_PUBLISH_4    "/TEF/tts300/attrs/s2"                                                      //            desses tópicos. Caso contrário, há grandes
                                                      //            chances de você controlar e monitorar o ESP32
                                                      //            de outra pessoa.
#define ID_MQTT  "fiware_300"   //id mqtt (para identificação de sessão)
                                 //IMPORTANTE: este deve ser único no broker (ou seja, 
                                 //            se um client MQTT tentar entrar com o mesmo 
                                 //            id de outro já conectado ao broker, o broker 
                                 //            irá fechar a conexão de um deles).
                                 // o valor "n" precisa ser único!
                                

// WIFI
const char* SSID = "FIAP-IBM"; // SSID / nome da rede WI-FI que deseja se conectar
const char* PASSWORD = "Challenge@23!"; // Senha da rede WI-FI que deseja se conectar
  
// MQTT
const char* BROKER_MQTT = "46.17.108.113"; //URL do broker MQTT que se deseja utilizar
int BROKER_PORT = 1883; // Porta do Broker MQTT
 
int D4 = 2;

//Variáveis e objetos globais
WiFiClient espClient; // Cria o objeto espClient
PubSubClient MQTT(espClient); // Instancia o Cliente MQTT passando o objeto espClient
char EstadoSaida = '0';  //variável que armazena o estado atual da saída


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


//Prototypes
void initSerial();
void initWiFi();
void initMQTT();
void reconectWiFi(); 
void mqtt_callback(char* topic, byte* payload, unsigned int length);
void VerificaConexoesWiFIEMQTT(void);
void InitOutput(void);

void apagaLed();

 
/* 
 *  Implementações das funções
 */
void setup() 
{
    //inicializações:
    InitOutput();
    initSerial();
    initWiFi();
    initMQTT();
    delay(5000);
    MQTT.publish(TOPICO_PUBLISH, "s|off");
    Serial.begin(115200);

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
  
//Função: inicializa comunicação serial com baudrate 115200 (para fins de monitorar no terminal serial 
//        o que está acontecendo.
//Parâmetros: nenhum
//Retorno: nenhum
void initSerial() 
{
    Serial.begin(115200);
}
 
//Função: inicializa e conecta-se na rede WI-FI desejada
//Parâmetros: nenhum
//Retorno: nenhum
void initWiFi() 
{
    delay(10);
    Serial.println("------Conexao WI-FI------");
    Serial.print("Conectando-se na rede: ");
    Serial.println(SSID);
    Serial.println("Aguarde");
     
    reconectWiFi();
}
  
//Função: inicializa parâmetros de conexão MQTT(endereço do 
//        broker, porta e seta função de callback)
//Parâmetros: nenhum
//Retorno: nenhum
void initMQTT() 
{
    MQTT.setServer(BROKER_MQTT, BROKER_PORT);   //informa qual broker e porta deve ser conectado
    MQTT.setCallback(mqtt_callback);            //atribui função de callback (função chamada quando qualquer informação de um dos tópicos subescritos chega)
}
  
//Função: função de callback 
//        esta função é chamada toda vez que uma informação de 
//        um dos tópicos subescritos chega)
//Parâmetros: nenhum
//Retorno: nenhum
void mqtt_callback(char* topic, byte* payload, unsigned int length) 
{
    String msg;
     
    //obtem a string do payload recebido
    for(int i = 0; i < length; i++) 
    {
       char c = (char)payload[i];
       msg += c;
    }
    
    Serial.print("- Mensagem recebida: ");
    Serial.println(msg);
    
    //toma ação dependendo da string recebida:
    //verifica se deve colocar nivel alto de tensão na saída D0:
    //IMPORTANTE: o Led já contido na placa é acionado com lógica invertida (ou seja,
    //enviar HIGH para o output faz o Led apagar / enviar LOW faz o Led acender)
    if (msg.equals("lamp108@on|"))
    {
        digitalWrite(D4, HIGH);
        EstadoSaida = '1';
    }
 
    //verifica se deve colocar nivel alto de tensão na saída D0:
    if (msg.equals("lamp108@off|"))
    {
        digitalWrite(D4, LOW);
        EstadoSaida = '0';
    }
}
  
//Função: reconecta-se ao broker MQTT (caso ainda não esteja conectado ou em caso de a conexão cair)
//        em caso de sucesso na conexão ou reconexão, o subscribe dos tópicos é refeito.
//Parâmetros: nenhum
//Retorno: nenhum
void reconnectMQTT() 
{
    while (!MQTT.connected()) 
    {
        Serial.print("* Tentando se conectar ao Broker MQTT: ");
        Serial.println(BROKER_MQTT);
        if (MQTT.connect(ID_MQTT)) 
        {
            Serial.println("Conectado com sucesso ao broker MQTT!");
            MQTT.subscribe(TOPICO_SUBSCRIBE); 
        } 
        else
        {
            Serial.println("Falha ao reconectar no broker.");
            Serial.println("Havera nova tentatica de conexao em 2s");
            delay(2000);
        }
    }
}
  
//Função: reconecta-se ao WiFi
//Parâmetros: nenhum
//Retorno: nenhum
void reconectWiFi() 
{
    //se já está conectado a rede WI-FI, nada é feito. 
    //Caso contrário, são efetuadas tentativas de conexão
    if (WiFi.status() == WL_CONNECTED)
        return;
         
    WiFi.begin(SSID, PASSWORD); // Conecta na rede WI-FI
     
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(100);
        Serial.print(".");
    }
   
    Serial.println();
    Serial.print("Conectado com sucesso na rede ");
    Serial.print(SSID);
    Serial.println("IP obtido: ");
    Serial.println(WiFi.localIP());
}
 
//Função: verifica o estado das conexões WiFI e ao broker MQTT. 
//        Em caso de desconexão (qualquer uma das duas), a conexão
//        é refeita.
//Parâmetros: nenhum
//Retorno: nenhum
void VerificaConexoesWiFIEMQTT(void)
{
    if (!MQTT.connected()) 
        reconnectMQTT(); //se não há conexão com o Broker, a conexão é refeita
     
     reconectWiFi(); //se não há conexão com o WiFI, a conexão é refeita
}
 
//Função: envia ao Broker o estado atual do output 
//Parâmetros: nenhum
//Retorno: nenhum
void EnviaEstadoOutputMQTT(void)
{
    if (EstadoSaida == '1')
    {
      MQTT.publish(TOPICO_PUBLISH, "s|on");
      Serial.println("- Led Ligado");
    }
    if (EstadoSaida == '0')
    {
      MQTT.publish(TOPICO_PUBLISH, "s|off");
      Serial.println("- Led Desligado");
    }
    Serial.println("- Estado do LED onboard enviado ao broker!");
    delay(1000);
}
 
//Função: inicializa o output em nível lógico baixo
//Parâmetros: nenhum
//Retorno: nenhum
void InitOutput(void)
{
    //IMPORTANTE: o Led já contido na placa é acionado com lógica invertida (ou seja,
    //enviar HIGH para o output faz o Led apagar / enviar LOW faz o Led acender)
    pinMode(D4, OUTPUT);
    digitalWrite(D4, HIGH);
    
    boolean toggle = false;

    for(int i = 0; i <= 10; i++)
    {
        toggle = !toggle;
        digitalWrite(D4,toggle);
        delay(200);           
    }

    digitalWrite(D4, LOW);

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
 
 
//programa principal
void loop() 
{   
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

    Serial.println(carroContador);

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
  
    const int potPin = 34;
    
    char msgBuffer[4];
    //garante funcionamento das conexões WiFi e ao broker MQTT
    VerificaConexoesWiFIEMQTT();
 
    //envia o status de todos os outputs para o Broker no protocolo esperado
    EnviaEstadoOutputMQTT();

    //carro
  
    dtostrf(carroContador, 2,1, msgBuffer);
    MQTT.publish(TOPICO_PUBLISH_2,msgBuffer);
    
    //keep-alive da comunicação com broker MQTT
    MQTT.loop();
}

// Para facilitar o código, apagando todos os leds e deixando apenas os necessários acesos!
void apagaLed(){
  digitalWrite(luzVermelhaCarros, LOW);
  digitalWrite(luzAmarelaCarros, LOW);
  digitalWrite(luzVerdeCarros, LOW);
  digitalWrite(luzVermelhaPedestres, LOW);
  digitalWrite(luzVerdePedestres, LOW);
}
