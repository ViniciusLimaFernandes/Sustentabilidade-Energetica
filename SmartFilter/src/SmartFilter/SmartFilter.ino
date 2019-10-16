#include <FirebaseCloudMessaging.h>
#include <FirebaseHttpClient.h>
#include <FirebaseError.h>
#include <FirebaseObject.h>
#include <Firebase.h>
#include <FirebaseArduino.h>

#include <Ethernet.h>

#include <FirebaseArduino.h>
 
#define FIREBASE_HOST "SmartFilter_PIIV"
#define FIREBASE_AUTH "AIzaSyC8XnpzlmYmYtW3Xr5VhV9jS6mYAoH6DxI"
#define WIFI_SSID "Lem"
#define WIFI_PASSWORD "naolembro63a"
 
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
  
  int sensorValue = analogRead(A0);   // Ler o pino Analógico A0 onde está o LDR
  float voltagem = sensorValue * (5 / 1024.0);   // Converter a leitura analógica (que vai de 0 - 1023) para uma voltagem (0 - 3.3V), quanto de acordo com a intensidade de luz no LDR a voltagem diminui.
  Serial.println(voltagem);   // Mostrar valor da voltagem no monitor serial
  Firebase.pushFloat("Tensao", voltagem); // Envia o dado da variavel voltagem para a variavel no Firebase
  delay(5000); 
  
}
