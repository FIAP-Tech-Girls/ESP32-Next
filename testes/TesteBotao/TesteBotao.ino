int teste;

int buttonPin = 18;

void setup()
{
  
    pinMode(buttonPin,  INPUT_PULLUP);  // Configura o pino do botão como entrada

    Serial.begin(9600);
}



void loop()
{
     // Verifica se o botão foi pressionado
  if (digitalRead(buttonPin) == HIGH) {
    teste = 1;
    
  }
  else{
    teste = 0;
  }
  Serial.println(teste);
}
