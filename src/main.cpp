#include "BluetoothSerial.h"

//Check if Bluetooth is enable
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

//Instance of BluetoothSerial
BluetoothSerial SerialBT;
const int ledPin = 2; // Definimos el pin del LED

//initialize Bluetooth serial comunication 
void setup() {
  Serial.begin(115200); //Start bluetooth module
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.printf("The device started, now you can pair it with bluetooth!");
  pinMode(ledPin, OUTPUT); // Inicializamos el pin del LED como salida
}

//Send and receive data
void loop() {
  digitalWrite(ledPin, HIGH); // Encendemos el LED
  Serial.printf("The device started, now you can pair it with bluetooth!");
  delay(500); // Esperamos 0.5 segundos
  digitalWrite(ledPin,LOW);
  delay(500);
  // If serialport receive data, then send data to device
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  // If there are bytes to read in serial port, write this data in serial monitor
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }
  delay(20);
}