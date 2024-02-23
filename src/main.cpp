#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define LED_PIN 2 // Pin del LED

BLECharacteristic *pCharacteristic;
bool state = false;

class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      Serial.println("Conectado al cliente BLE");
    };

    void onDisconnect(BLEServer* pServer) {
      Serial.println("Desconectado del cliente BLE");
    }
};

class MyCallbacks : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();

      if(value.length() == 1) {
        if(value[0] == '1') {
          digitalWrite(LED_PIN, HIGH);
          state = true;
        } else if(value[0] == '0') {
          digitalWrite(LED_PIN, LOW);
          state = false;
        }
      }
    }
};

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);

  BLEDevice::init("ESP32 BLE Device");
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(UUID("4fafc201-1fb5-459e-8fcc-c5c9c331914b"));

  pCharacteristic = pService->createCharacteristic(
                      UUID("beb5483e-36e1-4688-b7f5-ea07361b26a8"),
                      BLECharacteristic::PROPERTY_READ |
                      BLECharacteristic::PROPERTY_WRITE
                    );

  pCharacteristic->setCallbacks(new MyCallbacks());

  pService->start();
  pServer->getAdvertising()->start();
}

void loop() {
  delay(1000);

  if(state) {
    pCharacteristic->setValue("1");
  } else {
    pCharacteristic->setValue("0");
  }

  pCharacteristic->notify();
}