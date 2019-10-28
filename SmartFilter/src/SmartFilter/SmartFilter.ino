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

void RelayMod()//Funcao que define o ativamento/ desativamento das tomadas dependentes
{   
    delay(500);
    //Definicao de variaveis    
    double Irms2 = 0;
    double Irms3 = 0;
    double Diferenca = 0;
    double menor = 0;
    Voltage = getVPP();
    VRMS = (Voltage/2.0) *0.707; 
    
    //Calcula a corrente
    Irms2 = (((VRMS * 1000)/mVperAmp)- ruido);
    if(Irms2 < 0){
      Irms2 = 0;
    }

    //2 Segundos de delay
    delay(2000);

    //Calcula a corrente novamente
    Irms3 = (((VRMS * 1000)/mVperAmp)- ruido);
    if(Irms3 < 0){
      Irms3 = 0;
    }

    //Calcula a diferenca entre as medicoes realizadas
    if(Irms3 > Irms2){
      Diferenca = Irms3 - Irms2;
    }else{
      Diferenca = Irms2 - Irms3;
    }
    if(Irms2 == Irms3){
      Diferenca = 0;
    }

    //Verifica o menor valor lido
    if(Irms2<Irms3){
      menor = Irms2;
    }else{
      menor = Irms3;
    }

    //Condicoes para ativar/desativar as tomadas   
    if(menor < 0.2){
      digitalWrite(Pin_rele,HIGH); //Desativa tomadas
    }else if(Diferenca < 0.08){
      digitalWrite(Pin_rele,LOW); //Ativa tomadas
    }    
}

float getVPP()
{
  float result;
  
  int readValue;             //valor lido pelo sensor
  int maxValue = 0;          // variavel do valor max
  int minValue = 1024;          // variavel do valor min
  
   uint32_t start_time = millis();
   while((millis()-start_time) < 1000) //Amostra para 1 segundo
   {
       readValue = analogRead(Pin_acs712);
       // Verifica se existe algum outro valor maximo
       if (readValue > maxValue) 
       {
           /*salva o valor maximo*/
           maxValue = readValue;
       }
       if (readValue < minValue) 
       {
           /*salva o valor minimo*/
           minValue = readValue;
       }
   }
   
   // Subtrai o max do min
   result = ((maxValue - minValue) * 5.0)/1024.0;
      
   return result;
 }
