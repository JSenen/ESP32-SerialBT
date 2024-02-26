#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "DHT.h"

#define SERVICE_UUID        "6e400001-b5a3-f393-e0a9-e50e24dcca9e"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define DHTPIN              2     // Pin de datos del DHT11
#define DHTTYPE             DHT11 // Tipo de sensor DHT

BLEServer *pServer;
BLECharacteristic *pCharacteristic;
DHT dht(DHTPIN, DHTTYPE);

class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      Serial.println("Dispositivo BLE conectado");
    }

    void onDisconnect(BLEServer* pServer) {
      Serial.println("Dispositivo BLE desconectado");
    }
};

void setup() {
  Serial.begin(115200);
  dht.begin();

  BLEDevice::init("ESP32"); // Nombre del dispositivo BLE
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ |
                      BLECharacteristic::PROPERTY_NOTIFY
                    );
  pCharacteristic->addDescriptor(new BLE2902());
  pService->start();
  pServer->getAdvertising()->start();
  Serial.println("Servidor BLE iniciado");
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Error al leer el sensor DHT11");
    delay(2000);
    return;
  }

  pCharacteristic->setValue(String(temperature).c_str());
  pCharacteristic->notify();
  Serial.print("Temperatura enviada por BLE: ");
  Serial.print(temperature);
  Serial.println(" °C");

  delay(2000);
}
