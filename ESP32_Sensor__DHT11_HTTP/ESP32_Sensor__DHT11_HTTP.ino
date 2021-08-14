
#include <Arduino.h>
#include <HTTPClient.h>
#include "WiFi.h"

#include <Adafruit_Sensor.h>
#include <DHT.h>

// Credenciales de Red
const char* ssid = "iPhone de Carlos andres";
const char* password = "12345678";

String temp = "TEMPERATURE";
String humi = "HUMIDITY";

// Pin digital conectado al sensor DHT
#define DHTPIN 27

// Sensor de temperatura y humedad DHT 22 (AM2302) en uso
#define DHTTYPE    DHT22     // DHT 22 (AM2302)

DHT dht(DHTPIN, DHTTYPE);
/*--------------------------------
------Sensor Temp - Humidi--------
--------------------------------*/
String readDHTTemperature() {
  // Lecturas del sensor hastda 2 segundos
  // Lectura del sensor en grados Centigrados
  float t = dht.readTemperature();
  // Leer la temperatura como Fahrenheit (isFahrenheit = true)
  //float t = dht.readTemperature(true);

  if (isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
    Serial.println(t);
    return String(t);
  }
}

String readDHTHumidity() {
  // Lecturas del sensor hasta 2 segundos
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
    Serial.println(h);
    return String(h);
  }
}

// Marcador de posición con los valores de DHT
String processor(const String& var){
  //Serial.println(var);
  if(var == "TEMPERATURE"){
    return readDHTTemperature();
  }
  else if(var == "HUMIDITY"){
    return readDHTHumidity();
  }
  return String();
}

void setup(){
  // Monitor serial
  Serial.begin(115200);

  dht.begin();

  /*-------------------------
  ------conexion wifi--------
  -------------------------*/

  WiFi.begin(ssid, password);

  Serial.print("Conectando...");
  while (WiFi.status() != WL_CONNECTED) { //Check for the connection
  delay(500);
  Serial.print(".");
  }

  Serial.print("Conectado con éxito, mi IP es: ");
  Serial.println(WiFi.localIP());

}

void loop() {
HOLA
/*-------------------------
------conexion http--------
-------------------------*/
  if(WiFi.status()==WL_CONNECTED){
    HTTPClient http;
    String datos_a_enviar = "temp=" + temp + "&humi=" + humi;

    http.begin("https://domatikdisbeta01.ga/"); //Indicamos el destino
    http.addHeader("Content-Type", "application/x-www-form-urlencoded/"); //Preparamos el header text/plain si solo vamos a enviar texto plano sin un paradigma llave:valor.

    int codigo_respuesta = http.POST(datos_a_enviar);   //Enviamos el post pasándole, los datos que queremos enviar. (esta función nos devuelve un código que guardamos en un int)

    if(codigo_respuesta>0){
        Serial.println("Código HTTP ► " + String(codigo_respuesta));   //codigo de respuesta

        if(codigo_respuesta == 200){
          String cuerpo_respuesta = http.getString();
          Serial.println("El servidor respondió ▼ ");
          Serial.println(cuerpo_respuesta);

        }

      }else{

        Serial.print("Error enviando POST, código: ");
        Serial.println(codigo_respuesta);

      }

      http.end();  //libero recursos

    }else{

      Serial.println("Error en la conexión WIFI");

    }

    delay(2000);
  }
