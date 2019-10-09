#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define FIREBASE_HOST "host firebase"
#define FIREBASE_AUTH "key do firebase"
#define WIFI_SSID "user do wifi"
#define WIFI_PASSWORD "senha do wifi"

int led;
void setup() {
  //Define GPIO2 como saída
  pinMode (2, OUTPUT);
  //Inicia o monitor serial
  Serial.begin(9600);

  //Rotina pra conectar ao wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("conectando");
  while (WiFi.status() != WL_CONNECTED) {
	Serial.print(".");
	delay(500);
  }
  Serial.println();
  Serial.print("conectado: ");
  Serial.println(WiFi.localIP());
 
  //Iniciar Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
 //Receber a variavel led do firebase
 led = Firebase.getBool("led");
  Serial.print("led: ");
  Serial.println(led);
  if(led == 1){
	digitalWrite(2 ,HIGH);
  }
else if(led == 0){
	digitalWrite(2 ,LOW);
  }
}