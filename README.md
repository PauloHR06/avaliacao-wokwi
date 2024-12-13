# Prova prática - módulo 4

1. O primeiro passo foi padronizar todo o código em português, incluindo tanto comentários como variáveis e funções dentro do código.

2. As entradas e saídas também foram corrigidas, para os pinos corretos. O projeto não possuía um led azul e o led verde estava no pino 2, ao invés do 41.

**Antes**: 

```
#define led_azul 9 // Pino utilizado para controle do led azul
#define led_verde 41 // Pino utilizado para controle do led verda
#define led_vermelho 40 // Pino utilizado para controle do led vermelho
#define led_amarelo 9 // Pino utilizado para controle do led azul
```

**Depois**:

```
#define led_verde 2 // Pino utilizado para controle do led verde
#define led_vermelho 40 // Pino utilizado para controle do led vermelho
#define led_amarelo 9 // Pino utilizado para controle do led amarelo
```

3. Quando estiver no modo noturno, o protótipo deve piscar o led amarelo a cada segundo; Quando estiver claro (segundo a leitura analógica do sensor LDR), o protótipo deve ativar o modo convencional e fazer a temporização alternando entre verde (3 segundos), amarelo (2 segundos) e vermelho (5 segundos). Quando estiver claro (segundo a leitura analógica do sensor LDR) e o semáforo estiver no estado fechado (somente led vermelho aceso) e o botão for pressionado, o semáforo deve abrir 1 segundo após o pressionamento do botão. Quando estiver claro (segundo a leitura analógica do sensor LDR) e o semáforo estiver no estado fechado (somente led vermelho aceso) e o botão for pressionado 3 vezes, você deve enviar uma requisição HTTP para implementar um alerta (realize uma requisição para o http://www.google.com.br/ para fins de teste). Toda regra de negócio está expressa no código:

```
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
  ```

&emsp;O led amarelo pisca a cada segundo devido ao seguinte trecho de código:

```
// Pisca o led amarelo quando estiver escuro
digitalWrite(led_amarelo, HIGH);
delay(1000);
digitalWrite(led_amarelo, LOW);
delay(1000);
```

&emsp;O funcionamento do semáforo está presente nos seguintes trechos:

```
// 3 seg verde, 2 seg amarelo e 5 seg vermelho
digitalWrite(led_verde, HIGH);
delay(3000);
digitalWrite(led_verde, LOW);
digitalWrite(led_amarelo, HIGH);
delay(2000);
digitalWrite(led_amarelo, LOW);
digitalWrite(led_vermelho, HIGH);
delay(5000);
digitalWrite(led_vermelho, LOW);
```

&emsp; Caso o botão seja pressionado com o sinal vermelho aceso:

```
if (estadoBotao == HIGH) {
    delay(1000);
    digitalWrite(led_verde, HIGH);
}
```

&emsp;Caso o botão seja pressionado 3 vezes, será feita uma requisição POST http:

```
else if (contador >= 3) {
    // Adiciona uma requisição HTTP quando o botão for clicado 3 vezes
    http.begin(client, rotaServidor);
    http.addHeader("Alerta", "application/x-www-form-urlencoded");
    String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=BME280&value1=24.25&value2=49.54&value3=1005.14"; 
    int httpResponseCode = http.POST(httpRequestData);
}
```

4. A lógica do debouncing foi feita no seguinte trecho de código:
 ```
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
 ```
