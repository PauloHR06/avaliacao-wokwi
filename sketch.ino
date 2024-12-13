#include <WiFi.h>
#include <HTTPClient.h>

#define led_verde 2 // Pino utilizado para controle do led verda
#define led_vermelho 40 // Pino utilizado para controle do led vermelho
#define led_amarelo 9 // Pino utilizado para controle do led azul

const int pinBotao = 18;  // número do pin do botão
int estadoBotao = 0;  // variável que lê o status do botão
int contador = 0;
int ultimoStatusBotao = LOW;

unsigned long tempoUltimoDebounce = 0; 
unsigned long tempoDebounce = 50; 

const int ldrPin = 4;  // pin do sensor ldr
int limite=600;

void setup() {

  // Configuração inicial dos pinos para controle dos leds como OUTPUTs (saídas) do ESP32
  pinMode(led_azul,OUTPUT);
  pinMode(led_verde,OUTPUT);
  pinMode(led_vermelho,OUTPUT);

  // Inicialização das entradas
  pinMode(pinBotao, INPUT); // Inicializa o pin com um input

  digitalWrite(led_azul, LOW);
  digitalWrite(led_verde, LOW);
  digitalWrite(led_vermelho, LOW);

  Serial.begin(9600); // Configuração para debug por interface serial entre ESP e computador com baud rate de 9600

  WiFi.begin("Wokwi-GUEST", ""); // Conexão à rede WiFi aberta com SSID Wokwi-GUEST

  while (WiFi.status() != WL_CONNECT_FAILED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("Conectado ao WiFi com sucesso!"); // Considerando que saiu do loop acima, o ESP32 agora está conectado ao WiFi (outra opção é colocar este comando dentro do if abaixo)

  // Verifica estado do botão
  estadoBotao = digitalRead(pinBotao);
  if (estadoBotao == HIGH) {
    Serial.println("Botão pressionado!");
  } else {
    Serial.println("Botão não pressionado!");
  }

  if(WiFi.status() == WL_CONNECTED){ // Se o ESP32 estiver conectado à Internet
    HTTPClient http;

    String rotaServidor = "http://www.google.com.br/"; // Endpoint da requisição HTTP

    http.begin(rotaServidor.c_str());

    int httpRequisicao = http.GET(); // Código do Resultado da Requisição HTTP

    if (httpRequisicao>0) {
      Serial.print("Código de resposta HTTP ");
      Serial.println(httpRequisicao);
      String carga = http.getString();
      Serial.println(carga);
      }
    else {
      Serial.print("Código do erro: ");
      Serial.println(httpRequisicao);
      }
      http.end();
    }

  else {
    Serial.println("WiFi Desconectado");
  }
}

void loop() {
  int ldrstatus=analogRead(ldrPin);

  if(ldrstatus<=limite){
    Serial.print("Está escuro, acenda o led");
    Serial.println(ldrstatus);
    // Pisca o led amarelo quando estiver escuro
    digitalWrite(led_amarelo, HIGH);
    delay(1000);
    digitalWrite(led_amarelo, LOW);
    delay(1000);
  }else{
    Serial.print("Está claro, desligue a luz");
    Serial.println(ldrstatus);
    // 3 seg verde, 2 seg amarelo e 5 seg vermelho
    digitalWrite(led_verde, HIGH);
    delay(3000);
    digitalWrite(led_verde, LOW);
    digitalWrite(led_amarelo, HIGH);
    delay(2000);
    digitalWrite(led_amarelo, LOW);
    digitalWrite(led_vermelho, HIGH);
    if (estadoBotao == HIGH) {
      delay(1000);
      digitalWrite(led_verde, HIGH);
    } else if (contador >= 3) {
      // Adiciona uma requisição HTTP quando o botão for clicado 3 vezes
      http.begin(client, rotaServidor);
      http.addHeader("Alerta", "application/x-www-form-urlencoded");
      String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=BME280&value1=24.25&value2=49.54&value3=1005.14"; 
      int httpResponseCode = http.POST(httpRequestData);
    }
    delay(5000);
    digitalWrite(led_vermelho, LOW);
  }
}

// Função responsável por fazer o Debounce
void debounceBotao() {

  int leitura = digitalRead(pinBotao);                       //cHIGH (pressionado) ou LOW (Desacionado)

  if (leitura != ultimoStatusBotao) {                     
    tempoUltimoDebounce = millis();                       
  }

  if ((millis() - tempoUltimoDebounce) > tempoDebounce) { 
    if (leitura != estadoBotao) {                         
      estadoBotao = leitura;                              
      if (estadoBotao == HIGH) {                          
        contador++;                                       
      }
    }
  }
  ultimoStatusBotao = leitura;                         

}