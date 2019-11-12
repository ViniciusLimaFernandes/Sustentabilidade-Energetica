#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

//Definicoes
#define Pin_rele     D2             // Define o pino do rele
#define Pin_acs712   A0             // Define o pino do sensor de corrente
#define FIREBASE_HOST "x"
#define FIREBASE_AUTH "x"
#define WIFI_SSID "x"
#define WIFI_PASSWORD "x"

int Tensao = 127;
float ruido = 0.20;
int mVperAmp = 63; // Sensibilidade do ACS712
double Current = 0;
double VRMS = 0;
double AmpsRMS = 0;
 
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
  
  float valorTratado = trataValor();
  pushCurrent(valorTratado);
  wait(5);
  
}

void pushCurrent(float current){
  Firebase.pushFloat("current",current);
}

void wait(int sec){
  delay(sec * 1000);
}


void RelayMod()//Funcao que define o ativamento/ desativamento das tomadas dependentes
{   
    delay(500);
    //Definicao de variaveis    
    double Irms2 = 0;
    double Irms3 = 0;
    double Diferenca = 0;
    double menor = 0;
    Current = trataValor();
    VRMS = (Current/2.0) *0.707; 
    
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

float trataValor()
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
