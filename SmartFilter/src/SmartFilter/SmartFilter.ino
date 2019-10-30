#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define FIREBASE_HOST "x"
#define FIREBASE_AUTH "x"
#define WIFI_SSID "x"
#define WIFI_PASSWORD "x"
 
void setup() {
  Serial.begin(9600);
 
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("conectando");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("conectado: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}
void loop() {
  
  int sensorValue = analogRead(A0);   // Ler o pino Analógico A0 onde está o sensor
  float valorTratado = trataValor(sensorValue);
  pushTensao(valorTratado);
  wait(5);
  
}

float trataValor(int valorLido){
   float result = valorLido * (5/1024);
   Serial.println(result);
   return result;
}

void pushTensao(float tensao){
  Firebase.pushFloat("Tensao",tensao);
}

void wait(int sec){
  delay(sec * 1000);
}
