#include <Arduino.h>
#include <ArduinoJson.h>
#include "BluetoothSerial.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 2  
//#define LED 13

#define DHTTYPE DHT11   // DHT 11
#define SERVICE_UUID "00001101-0000-1000-8000-00805F9B34FB"

DHT dht(DHTPIN, DHTTYPE);

//Check if Bluetooth is enable
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

//Instance of BluetoothSerial
BluetoothSerial SerialBT;
//const int ledPin = 3; // Definimos el pin del LED

//initialize Bluetooth serial comunication 
void setup() {
  Serial.begin(115200); //Start bluetooth module
  SerialBT.begin("ESP32test"); //Bluetooth device name
  dht.begin(); // Begin DHTC11 Temperature sensor
  Serial.println("The device started, now you can pair it with bluetooth!");
 // pinMode(LED, OUTPUT); // Inicializamos el pin del LED como salida
}

//Send and receive data
void loop() {
  if (SerialBT.available()) {
    // Leer los datos recibidos en un buffer
    uint8_t buffer[32];
    size_t size = SerialBT.readBytes(buffer, sizeof(buffer));

    // Convertir los bytes recibidos en una cadena hexadecimal
String receivedHex;
for (size_t i = 0; i < size; i++) {
  // Convierte el byte en su representación hexadecimal como una cadena
  String hexByte = String(buffer[i], HEX);
  // Asegúrate de que el byte se represente con dos caracteres
  if (hexByte.length() == 1) {
    hexByte = "0" + hexByte; // Agrega un 0 al principio si es necesario
  }
  // Concatena la representación hexadecimal del byte a la cadena recibida
  receivedHex += hexByte;
}
Serial.println("HEX recibido = " + receivedHex);

    // Comparar la cadena hexadecimal recibida con el código esperado
    if (receivedHex == "0207e20103ef04") {
      // Enviar ACK correcto
      SerialBT.print("020606031104\r\n");
    } else {
      // Enviar NACK
      SerialBT.print("020615032004\r\n");
    }
  }
}
