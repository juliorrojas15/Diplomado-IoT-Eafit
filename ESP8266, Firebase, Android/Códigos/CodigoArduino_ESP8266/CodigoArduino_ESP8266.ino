//
// Copyright 2015 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

// FirebaseDemo_ESP8266 is a sample that demo the different functions
// of the FirebaseArduino API.

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// Set these to run example.
#define FIREBASE_HOST "iot-sena.firebaseio.com"
#define FIREBASE_AUTH "N3sQMc969ykBohrDW6FhLzTbqlhKTlYbT0jFbbdE"
#define WIFI_SSID "Familia Rojas"
#define WIFI_PASSWORD "3123888490"

//Variables Globales
int iBoton=15;
int iLed=5;

void setup() {
  Serial.begin(9600);
  //Configuración de Entradas Salidas
  pinMode(iBoton,INPUT);
  pinMode(iLed,OUTPUT);
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

int n = 0;
boolean xFlanco=false;
boolean xLedEncendidoApp=false;
boolean xVarBotonFisico=false;
boolean xVarBotonApp=false;
int iContador=0;
int iBucles=0;

void loop() {

  //Lectura de señales digitales
  boolean xEstadoBoton=digitalRead(iBoton);
  boolean xEstadoLed=digitalRead(iLed);

  //Accionamiento de botón fisico
  if(xEstadoBoton==1 && xFlanco==0){
    digitalWrite(iLed,!xEstadoLed);
    xVarBotonFisico=!xVarBotonFisico;
    iContador++;    
    fSet(xVarBotonFisico);
    fSetInt(iContador);   
  }
  xFlanco=xEstadoBoton;    

  //Accionamiento de switch de App
  boolean xVarBotonApp=Firebase.getBool("Boton App");
  delay(200);
  if(xVarBotonApp==true && xLedEncendidoApp==false){
     digitalWrite(iLed,true);
     xLedEncendidoApp=true;
   }
   if(xVarBotonApp==false && xLedEncendidoApp==true){
     digitalWrite(iLed,false);
     xLedEncendidoApp=false; 
   }

  //Al completar 5 accionamientos eliminamos el contador y creamos un bucle
  if(iContador==5){
    iBucles++;
    fRemove();
    fPush();
    iContador=0;
  }

}

//##############################################  set value
void fSet(boolean xValor){
    Firebase.setBool("Boton Físico",xValor );
    // handle error
    if (Firebase.failed()) {
        Serial.print("setting /number failed:");
        Serial.println(Firebase.error());  
        return;
    }
}  
 

//##############################################  set integer
void fSetInt(int iValor){
    Firebase.setInt("Mensaje a app",iValor );
    // handle error
    if (Firebase.failed()) {
        Serial.print("setting /number failed:");
        Serial.println(Firebase.error());  
        return;
    }
}  

//##############################################  remove register
void fRemove(){
  Firebase.remove("Contador");
}

//##############################################  Push register
void fPush(){
  String name = Firebase.pushInt("logs", iBucles);
  // handle error
  if (Firebase.failed()) {
      Serial.print("pushing /logs failed:");
      Serial.println(Firebase.error());  
      return;
  }
}

//##############################################  Get value
void fGet(){
}


  /*
  // set value
  Firebase.setFloat("number", 42.0);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);
  
  // update value
  Firebase.setFloat("number", 43.0);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);

  // get value 
  Serial.print("number: ");
  Serial.println(Firebase.getFloat("number"));
  delay(1000);

  // remove value
  
  delay(1000);

  // set string value
  Firebase.setString("message", "hello world");
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /message failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);
  
  // set bool value
  Firebase.setBool("truth", false);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /truth failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);

  // append a new value to /logs
  String name = Firebase.pushInt("logs", n++);
  // handle error
  if (Firebase.failed()) {
      Serial.print("pushing /logs failed:");
      Serial.println(Firebase.error());  
      return;
  }
  Serial.print("pushed: /logs/");
  Serial.println(name);
  delay(1000);
  
}*/
