/////////////////////////////////
// Generated with a lot of love//
// with TUNIOT FOR ESP32     //
// Website: Easycoding.tn      //
/////////////////////////////////
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

BLECharacteristic *Char_BTSerialTX;

String DeveiceName = "BeNoFi System";

// GATT Serivce UUID - Generic Access(0X1800)
#define BeNoFi_System_UUID "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
// Service Classes and Profiles - RX SerialPort (0x1102)
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
// Service Classes and Profiles - TX SerialPort (0x1103)
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

String RXDATA;

const int LED = 4;

BLEServer *pServer = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;
int txValue = 0;

class ServerCallbacks : public BLEServerCallbacks
{
  void onConnect(BLEServer *pServer)
  {
    deviceConnected = true;
  };

  void onDisconnect(BLEServer *pServer)
  {
    deviceConnected = false;
  }
};

void BeNoFiSystem(String)
{
  Serial.print("RXDATA: ");
  Serial.println(RXDATA);

  digitalWrite(LED, HIGH);
  delay(2000);
  digitalWrite(LED, LOW);
}

class Callbacks : public BLECharacteristicCallbacks
{
  void onWrite(BLECharacteristic *Char_BTSerialRX)
  {
    std::string rxValue = Char_BTSerialRX->getValue();

    // Clear RXDATA for processing
    RXDATA = "";

    if (rxValue.length() > 0)
    {
      Serial.println("*********");
      Serial.print("Received Value: ");
      for (int i = 0; i < 3 ; i++)
      {
        Serial.print(rxValue[i]);
        RXDATA += rxValue[i];
      }
      Serial.println();
      Serial.println("*********");
    }
    // Injecting Benofi System here.
    if (RXDATA == "R2T$" || RXDATA == "T2R$" || RXDATA == "R2T" || RXDATA == "T2R")
    {
      BeNoFiSystem(RXDATA);
    }
  }
};

void setup()
{
  Serial.begin(115200);
  Serial.println("Start");
  pinMode(LED, OUTPUT);

  BLEDevice::init("BeNoFi System");
  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new ServerCallbacks());

  // Create the BLE Service
  BLEService *pBenofi_System = pServer->createService(BeNoFi_System_UUID);

  BLECharacteristic *Char_BTSerialRX = pBenofi_System->createCharacteristic(
      CHARACTERISTIC_UUID_RX,
      BLECharacteristic::PROPERTY_WRITE);

  Char_BTSerialTX = pBenofi_System->createCharacteristic(
      CHARACTERISTIC_UUID_TX,
      BLECharacteristic::PROPERTY_NOTIFY);

  Char_BTSerialTX->addDescriptor(new BLE2902());

  Char_BTSerialRX->setCallbacks(new Callbacks());

  // Start advertising
  pBenofi_System->start();

  pServer->getAdvertising()->addServiceUUID(BeNoFi_System_UUID);
  pServer->getAdvertising()->start();

  Serial.println("Waiting a client connection to notify...");
}

void loop()
{
  if (deviceConnected)
  {
    Char_BTSerialTX->setValue("Connected");
    Char_BTSerialTX->notify();
    delay(10); // bluetooth stack will go into congestion, if too many packets are sent
  }

  // disconnecting
  if (!deviceConnected && oldDeviceConnected)
  {
    delay(500);                  // give the bluetooth stack the chance to get things ready
    pServer->startAdvertising(); // restart advertising
    Serial.println("start advertising");
    oldDeviceConnected = deviceConnected;
  }
  // connecting
  if (deviceConnected && !oldDeviceConnected)
  {
    // do stuff here on connecting
    oldDeviceConnected = deviceConnected;
  }
}