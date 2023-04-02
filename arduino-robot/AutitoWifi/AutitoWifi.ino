#include <BearSSLHelpers.h>
#include <CertStoreBearSSL.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiGratuitous.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiClientSecureAxTLS.h>
#include <WiFiClientSecureBearSSL.h>
#include <WiFiServer.h>
#include <WiFiServerSecure.h>
#include <WiFiServerSecureAxTLS.h>
#include <WiFiServerSecureBearSSL.h>
#include <WiFiUdp.h>

#include <Arduino.h>
#include <espnow.h>

// Definición de constantes
#define LED LED_BUILTIN
#define IN1 D7
#define IN2 D8
#define ENA D6
#define IN3 D1
#define IN4 D3
#define ENB D2
#define DELAY_MS 50
#define PWM_VALUE 600
#define PWM_VALUE2 800

// Prototipos de funciones
void control_motores(char control);
void onDataReceiver(uint8_t * mac, uint8_t *incomingData, uint8_t len);

// Variables globales
uint8_t key[] =  {0x33, 0x44, 0x33, 0x44, 0x33, 0x44, 0x33, 0x44, 0x33, 0x44, 0x33, 0x44, 0x33, 0x44, 0x33, 0x44};
char myMessage;
char control;
char bandera = 0;

void setup() {
  // Configurar los puertos
  pinMode(IN1,OUTPUT);   //left motor forward
  pinMode(IN2,OUTPUT);   //left motor reverse
  pinMode(IN3,OUTPUT);   //right motor forward
  pinMode(IN4,OUTPUT);   //right motor reverse
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  //Configuración inicial de los motores apagados
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, PWM_VALUE);
  analogWrite(ENB, PWM_VALUE);

  // Configuración Serial y Wifi
  Serial.begin(9600);
  WiFi.disconnect();
  ESP.eraseConfig();
  WiFi.mode(WIFI_STA);
  Serial.print("Mac Address: ");
  Serial.print(WiFi.macAddress());
  Serial.println("\nESP-Now Receiver");
  
  // Initializing the ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Problem during ESP-NOW init");
    return;
  }

  // Configuración de las luces
  pinMode(LED,OUTPUT);
  digitalWrite(LED, LOW);

  Serial.println("Iniciando Autito fantástico");
  
  //esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  // We can register the receiver callback function
  esp_now_register_recv_cb(onDataReceiver);
}
void loop() {
  // put your main code here, to run repeatedly:
}

void onDataReceiver(uint8_t * mac, uint8_t *incomingData, uint8_t len){
  Serial.println("Message received.");
   // We don't use mac to verify the sender
   // Let us transform the incomingData into our message structure
  memcpy(&myMessage, incomingData, sizeof(myMessage));
  Serial.println(myMessage); 
  control_motores(myMessage);
}

void control_motores(char control){
  switch(control){           // Testear entrada 
    case 'A': // Ir hacia delante
      //Serial.println("Ir hacia adelante");
      //digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN4, LOW);
      //delay(2);
      digitalWrite(IN1, HIGH);
      digitalWrite(IN3, HIGH);
      analogWrite(ENA, PWM_VALUE);
      analogWrite(ENB, PWM_VALUE);
      //digitalWrite(IN4, LOW);
      
      delay(1000);
      break;
    case 'R': // Ir hacia atras
      //Serial.println("Ir hacia atras");
      digitalWrite(IN1, LOW); // Motor derecha
      digitalWrite(IN3, LOW);
      //digitalWrite(IN2, HIGH);
      //delay(2);      
      //digitalWrite(IN3, LOW); // Motor izquierda
      digitalWrite(IN4, HIGH);
      digitalWrite(IN2, HIGH);
      analogWrite(ENA, PWM_VALUE);
      analogWrite(ENB, PWM_VALUE);
      delay(1000);
      break;
    case 'D': // Girar a la derecha
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      delay(2);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      analogWrite(ENA, PWM_VALUE2);
      analogWrite(ENB, PWM_VALUE2);
      delay(1000);
      break;
    case 'I': // Girar a la izquierda
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      delay(2);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      analogWrite(ENA, PWM_VALUE2);
      analogWrite(ENB, PWM_VALUE2);
      delay(1000);
      break;
    case 'L': // Encender/Apagar luces
      if(bandera){
        digitalWrite(LED, LOW);
        bandera = 0;
      }else{
        digitalWrite(LED, HIGH);
        bandera = 1;
      }
      break;
    case '0':
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
      break;
    default:
      break;
  }
}
