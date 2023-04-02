#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include "spectrogram.h"
#include "speech_commands_model.h"

/* Declaración de constantes */
#define SENSOR_PIN 34
#define BOTON_PIN 5

/* Prototipos de funciones */
void protocol_esp_now_init(void);
void grabar_audio(void);
void imprimir_audio(void);
void imprimir_espectrograma(void);

float * audio;
float * espectrograma;

data_t input = { SPEECH_COMMANDS_MODEL_CHANNELS, SPEECH_COMMANDS_MODEL_WIDTH, SPEECH_COMMANDS_MODEL_HEIGHT, espectrograma };
flatten_data_t resultados; 

// ESP8266 Mac address
uint8_t mac_peer1[] = {0x48, 0x3F, 0xDA, 0x44, 0x00, 0x14};
esp_now_peer_info_t peer1;

char myMessage;

char * predicciones[] = {"Stop", "Go", "Right", "Left", "Down"};

void setup() {
  Serial.begin(115200);
  model_init();
  protocol_esp_now_init();

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  pinMode(SENSOR_PIN, INPUT);
  pinMode(BOTON_PIN, INPUT);
  delay(1000);
}

void loop() {
  audio = (float*)swap_alloc(sizeof(float)*FRAME_LENGTH);
  espectrograma = (float*)swap_alloc(sizeof(float)*SPEC_SIZE);
  grabar_audio();
  unsigned long tiempo = micros();
  create_spectrogram(audio, espectrograma);
  input.data = espectrograma;
  uint8_t prediccion = model_predict(input,&resultados);
  tiempo = micros() - tiempo;
  
  Serial.println("Send a new message");
  switch(prediccion){
    case 1:
      myMessage = 'A'; break;
    case 2:
      myMessage = 'D'; break;
    case 3:
      myMessage = 'I'; break;
    case 4:
      myMessage = 'R'; break;
    case 0:
      myMessage = '0'; break;
    default:
      myMessage = '0'; break;
  }
  Serial.println(myMessage);
  esp_now_send(NULL, (uint8_t *) &myMessage, sizeof(myMessage));
  
  //imprimir_audio();
  //imprimir_espectrograma();
  Serial.print("Predicción: ");
  Serial.println(predicciones[prediccion]);
  Serial.print("Confianza: ");
  Serial.println(resultados.data[prediccion]);
  Serial.print("Tiempo de prediccion: ");
  Serial.println(tiempo);
  Serial.println();
  
  delay(200);
}

void protocol_esp_now_init(){
  WiFi.mode(WIFI_STA);
  
  // Get Mac Add
  Serial.print("Mac Address: ");
  Serial.print(WiFi.macAddress());
  Serial.println("ESP32 ESP-Now Broadcast");
  
  // Initializing the ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Problem during ESP-NOW init");
    return;
  }
  
  memcpy(peer1.peer_addr, mac_peer1, 6);
  peer1.channel = 4;
  peer1.encrypt = 0;
  
  // Register the peer
  Serial.println("Registering a peer 1");
  if ( esp_now_add_peer(&peer1) == ESP_OK) {
    Serial.println("Peer 1 added");
  }  
}
void grabar_audio(){
  char c = 0;
  float prom=0;
  uint16_t valor;
  int i,j=0;
  int listo;
  int boton = 0;

  Serial.println("Presione el boton y pronuncie su orden");
  while(boton != 1){
    boton = digitalRead(BOTON_PIN);
    delay(20);
  }

  // Grabación de voz
  listo = 0;
  digitalWrite(LED_BUILTIN, HIGH);
  for(i=0;i<FRAME_LENGTH;i++){
    valor = analogRead(SENSOR_PIN);
    audio[i] = valor;
    prom += valor;
    delayMicroseconds(DELAY_US);
  }
  digitalWrite(LED_BUILTIN, LOW);

  prom /= FRAME_LENGTH;

  // Resto componente de continua
  for(i=0;i<FRAME_LENGTH;i++){
    audio[i] -= prom;
  }
}

void imprimir_audio(){
  Serial.println("audio_vec = np.array([");
  Serial.print(audio[0]);
  for(int i=1;i<FRAME_LENGTH;i++){
    Serial.print(", ");
    Serial.print(audio[i]);
  }
  Serial.println("])");
  Serial.println();
}

void imprimir_espectrograma(){
  Serial.println("espectrograma_vec = np.array([");
  Serial.print(espectrograma[0]);
  for(int i=1;i<SPEC_SIZE;i++){
    Serial.print(", ");
    Serial.print(espectrograma[i]);
  }
  Serial.println("])");
  Serial.println("espectrograma_vec = np.reshape(espectrograma_vec, (bloques, n_mels))");
  Serial.println();
}
