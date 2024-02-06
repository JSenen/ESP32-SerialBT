#include "BluetoothSerial.h"

//Check if Bluetooth is enable
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

//Instance of BluetoothSerial
BluetoothSerial SerialBT;

//initialize Bluetooth serial comunication 
void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
}

//Send and receive data
void loop() {
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