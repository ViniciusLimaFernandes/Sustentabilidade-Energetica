#include <FirebaseCloudMessaging.h>
#include <FirebaseHttpClient.h>
#include <FirebaseError.h>
#include <FirebaseObject.h>
#include <Firebase.h>
#include <FirebaseArduino.h>

#include <Ethernet.h>

#include <FirebaseArduino.h>
 
#define FIREBASE_HOST "SmartFilter_PIIV"
#define FIREBASE_AUTH "firebaseAuth"
#define WIFI_SSID "wifiName"
#define WIFI_PASSWORD "wifiPass"
 
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

void myTimerEvent()// Funcao com calulos basicos de corrente/ potencia e etc...
{  
    //Calculos
    Voltage = getVPP(); // Tenso mV
    VRMS = (Voltage/2.0) * 0.707; // Calculo da tensao RMS
    AmpsRMS = (((VRMS * 1000)/mVperAmp)- ruido); //Calculo da corrente
    double Pot = AmpsRMS * Tensao; //Potencia
    double kWH = (Pot * 1)/1000;

    //Codicoes para evitar numeros negativos 
    if(AmpsRMS < 0){
      AmpsRMS = 0;
    }

    if(Pot < 0){
      Pot = 0;
    }

    if(kWH < 0){
     kWH = 0; 
    }

    //Prints e envios de dados
    Serial.print(AmpsRMS);
    Serial.println(" Amps RMS");
    Serial.print(Voltage);
    Serial.println(" V");

    Blynk.virtualWrite(V0, AmpsRMS);
    Blynk.virtualWrite(V1, Pot);
    Blynk.virtualWrite(V2, kWH);

    
}
