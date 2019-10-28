#include <FirebaseCloudMessaging.h>
#include <FirebaseHttpClient.h>
#include <FirebaseError.h>
#include <FirebaseObject.h>
#include <Firebase.h>
#include <FirebaseArduino.h>

#include <Ethernet.h>

#include <FirebaseArduino.h>
 
#define FIREBASE_HOST "SmartFilter_PIIV"
#define FIREBASE_AUTH "xxxxxxxx"
#define WIFI_SSID "xxxxx"
#define WIFI_PASSWORD "xxxxx"
 
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
   Serial.printl(result);
   return result;
}

void pushTensao(float tensao){
  Firebase.pushFloat("Tensao",tensao);
}

void wait(int sec){
  delay(sec * 1000);
}
