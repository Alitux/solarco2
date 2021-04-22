/****************************************
/ FIRMWARE VERSION: 1.0C
/****************************************
 * Solar Sensor CO2 con Autoconexión y actualización OTA
 * 1xSCD30 (D2)
 * MQTT (Thingsboard)
 * Alitux -22/04/21-
 * ¡NO DEEP SLEEP!
 */

/****************************************
 * Librerias
 ****************************************/
#include "Adafruit_Sensor.h"
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         // https://github.com/tzapu/WiFiManager
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ThingsBoard.h>
#include <Wire.h>
#include <SparkFun_SCD30_Arduino_Library.h>




/****************************************
 * Constantes
 ****************************************/
#define DEVICENAME "SolarCO2"
char thingsboardServer[] = "1.2.3.4"; //IP del servidor donde esta instalado Thingsboard
#define TOKEN "12345678912345" //Cambiar TOKEN por el que brinda Thingsboard.
unsigned long frecuencia = 15000; // (15 segundos) Frecuencia de muestreo en ms TEST
unsigned long previousMillis;
int primertoma;

WiFiClient espClient;
SCD30 airSensor;

// Parametros WifiManager
WiFiServer server(80);
// Variable to store the HTTP request
String header;
unsigned long lastMillis = 0;
ThingsBoard tb(espClient);
int status = WL_IDLE_STATUS;
unsigned long lastSend;

/****************************************
 * Auxiliar Functions
 ****************************************/



/****************************************
 * Main Functions
 ****************************************/
void tomadatos(){
  // Función para toma de datos.
  // Lectura SCD30 1

  if (airSensor.dataAvailable())
 {
   float h1 = airSensor.getHumidity();
   float t1 = airSensor.getTemperature();
   float co2 = airSensor.getCO2();

    tb.sendTelemetryFloat("temperatura", t1);
    tb.sendTelemetryFloat("humedad", h1);
    tb.sendTelemetryFloat("co2", co2);
 }
 else
   Serial.println("Esperando datos nuevos");

   delay(500);
}

void setup() {
  // Código que se ejecuta una vez
  previousMillis = millis();
  primertoma = 1;
  Serial.begin(115200);
  wifi_station_set_hostname(DEVICENAME);
  // WiFiManager
  WiFiManager wifiManager;
  // Descomentar para borrar redes guardadas
  // wifiManager.resetSettings();
  // Nombre de la red igual a la del device
  wifiManager.autoConnect(DEVICENAME);
  // Si está conectado...
  Serial.println("Conectado");
  ArduinoOTA.onStart([]() {
  String type;
  if (ArduinoOTA.getCommand() == U_FLASH) {
    type = "sketch";
  } else { // U_SPIFFS
    type = "filesystem";
  }
    // ADAPTACION PARA OTA
    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Wire.begin();
  if (airSensor.begin() == false)
  {
    Serial.println("Sensor de aire no detectado. Revisar conexiones. Freezing...");
    while (1)
      ;
  }

}

void loop() {
  // Código central:
  // TOMA DATOS INICIAL
  if ( !tb.connected() ) {
    reconnect();
  }
  if (primertoma == 1) {
    Serial.println("Muestra Inicial");
    tomadatos();
    primertoma = 0;
    }
  // TOMA DE DATOS POR FRECUENCIA
  unsigned long currentMillis = millis();
  if ((unsigned long)(currentMillis - previousMillis) >= frecuencia) {
    tomadatos();
    previousMillis = millis();
  }
  tb.loop();
  ArduinoOTA.handle();
}

void reconnect() {
  // Loop until we're reconnected
  while (!tb.connected()) {
    Serial.print("Conectando al servidor ...");
    // Attempt to connect (clientId, username, password)
    if ( tb.connect(thingsboardServer, TOKEN) ) {
      Serial.println( "[DONE]" );
    } else {
      Serial.print( "[FAILED]" );
      Serial.println( " : retrying in 5 seconds" );
      // Wait 5 seconds before retrying
      delay( 5000 );
    }
  }
}
