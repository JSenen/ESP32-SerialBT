#include <Arduino.h>
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
 // digitalWrite(LED, HIGH); // Encendemos el LED
  delay(500); // Esperamos 3 segundos entre lecturas

  float h = dht.readHumidity();
  // Leer temperatura ºC
  float t = dht.readTemperature();
  // si se le pasa a la funcion el parametro true obtenemos la temperatura en ºF
  float f = dht.readTemperature(true);

  Serial.println("Humedad: ");
  Serial.print(h);
  Serial.print("%\n");
  Serial.println("Temperatura: ");
  Serial.print(t);
  Serial.println("°C  ");
  Serial.print(f);
  Serial.println("°F");

  //Transferir los datos por Bluetooth
  SerialBT.print(t);
  
  //digitalWrite(LED,LOW);
  delay(500);

  // If serialport receive data, then send data to device
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  // If there are bytes to read in serial port, write this data in serial monitor
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }
  delay(200);
}