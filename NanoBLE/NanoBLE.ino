#include <ArduinoBLE.h>

BLEService bikeService("19B10000-E8F2-537E-4F6C-D104768A1214");

BLEByteCharacteristic bikeCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

int counter = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("Failed to start BLE");
    while(1);
  }

  // set advertised local name and service UUID:
  BLE.setLocalName("BIKE");
  BLE.setAdvertisedService(bikeService);

  // add the characteristic to the service
  bikeService.addCharacteristic(bikeCharacteristic);

  // add service
  BLE.addService(bikeService);

  // set the initial value for the characeristic:
  bikeCharacteristic.writeValue(counter);

  // start advertising
  BLE.advertise();

  Serial.println("Bluetooth device active. Waiting for connections ...");

}

void loop() {
  // listen for BLE peripherals to connect:
  BLEDevice central = BLE.central();

  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());

    // while the central is still connected to peripheral:
    while (central.connected()) {
      // if the remote device wrote to the characteristic,
      // use the value to control the LED:
      if (bikeCharacteristic.written()) {
        counter = bikeCharacteristic.value();
        Serial.print("Valor del contador leido: ");
        Serial.println(counter);
      }
    }
    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
}
