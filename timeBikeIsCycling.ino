//timeBikeIsCycling

//Programm that calculates the time a bike is cycling. It gets the seconds is cycling and send this time
//when is demanded externaly. Each time the time is demanded, it goes to 0.
//During the Bluetooth connexion with the phone, the count is stopped

//V1.0 Fernando Jarilla 09 - 2019

#include <Wire.h>
#include <ArduinoBLE.h>
#include <Arduino_LSM9DS1.h>
#include <Arduino_APDS9960.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Bluetooth module. Preparing counter.
//How the BLEArduino can't put more than a char in one characteristic, we get one characteristic 
//for each digit of the counter. We can count from 0 to 99.999.999 seconds without reset the counter.
// Char0 Char1 Char2 Char3 Char4 Char5 Char6 Char7
// MSB   ...   ...   ...   ...   ...   ...   LSB      (Counter)
BLEService bikeService("0000");
BLECharCharacteristic bikeCharacteristic0("0001", BLERead | BLEWrite);
BLECharCharacteristic bikeCharacteristic1("0002", BLERead | BLEWrite);
BLECharCharacteristic bikeCharacteristic2("0003", BLERead | BLEWrite);
BLECharCharacteristic bikeCharacteristic3("0004", BLERead | BLEWrite);
BLECharCharacteristic bikeCharacteristic4("0005", BLERead | BLEWrite);
BLECharCharacteristic bikeCharacteristic5("0006", BLERead | BLEWrite);
BLECharCharacteristic bikeCharacteristic6("0007", BLERead | BLEWrite);
BLECharCharacteristic bikeCharacteristic7("0008", BLERead | BLEWrite);

//Global constants
//set counter
unsigned long timeCounter;
char timeCounterString[8];
//inputs for mouvement sensor
float x, y, z; 
float antX=0, antY=0, antZ=0;

void setup(){
  //set monitor
  //Serial.begin(115200);
  //while(!Serial);

  //Bluetooth module
  //Begin initialization
  if (!BLE.begin()) {
    //Serial.println("Failed to start BLE");
    while(1);
  }

  //IMU module
  //Begin initialization
  if (!IMU.begin()) {
    //Serial.println("Failed to initialize IMU");
    while(1);
  }
  
  //Set advertised local name and service UUID:
  BLE.setLocalName("BIKE");
  BLE.setAdvertisedService(bikeService);

  //Add the characteristic to the service (the counter)
  bikeService.addCharacteristic(bikeCharacteristic0);
  bikeService.addCharacteristic(bikeCharacteristic1);
  bikeService.addCharacteristic(bikeCharacteristic2);
  bikeService.addCharacteristic(bikeCharacteristic3);
  bikeService.addCharacteristic(bikeCharacteristic4);
  bikeService.addCharacteristic(bikeCharacteristic5);
  bikeService.addCharacteristic(bikeCharacteristic6);
  bikeService.addCharacteristic(bikeCharacteristic7);

  //Add service
  BLE.addService(bikeService);

  //Assing event handlers for chracteristic. We handle the characteristic with the last characteristic
  //envoyed.
  //Now is not necessary. The phone read the value when it want
  bikeCharacteristic7.setEventHandler(BLEWritten, bikeCharacteristicWritten);

  //Start advertising
  BLE.advertise();

  //Inform that module is initialized.
  //Serial.println("Bluetooth device active. Waiting for connections ...");
  //Serial.println("IMU device active.");
}//end setup()

//Activation with a characteristic is written. We dont do anything in this case. We write the characteristics
//value every second, not when change
//Not necessary at this moment

void bikeCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic) {
  //New value is wrotten to characteristic
  readTimeCounter();
}


//Reading of time counter. We read the characteristics and put the value in int in the counter
void readTimeCounter() {
  timeCounterString[7] = bikeCharacteristic0.value();
  timeCounterString[6] = bikeCharacteristic1.value();
  timeCounterString[5] = bikeCharacteristic2.value();
  timeCounterString[4] = bikeCharacteristic3.value();
  timeCounterString[3] = bikeCharacteristic4.value();
  timeCounterString[2] = bikeCharacteristic5.value();
  timeCounterString[1] = bikeCharacteristic6.value();
  timeCounterString[0] = bikeCharacteristic7.value();        
  timeCounter = atol(timeCounterString);
}

//Writting the characteristics. We get the counter value (int) and pass to eight digits. We write every
//characteristic with this digits
void writeTimeCounterString() {
  utoa(timeCounter, timeCounterString, 10);
  bikeCharacteristic0.writeValue(timeCounterString[7]);
  bikeCharacteristic1.writeValue(timeCounterString[6]);
  bikeCharacteristic2.writeValue(timeCounterString[5]);
  bikeCharacteristic3.writeValue(timeCounterString[4]);
  bikeCharacteristic4.writeValue(timeCounterString[3]);
  bikeCharacteristic5.writeValue(timeCounterString[2]);
  bikeCharacteristic6.writeValue(timeCounterString[1]);
  bikeCharacteristic7.writeValue(timeCounterString[0]);
}

void loop(){  
  //Listen for BLE peripherals to connect:
  BLEDevice central = BLE.central();
  //If a central is connected to peripheral:
  if (central) {
    central.poll();
    Serial.print("Connected to central: ");
    //Print the central's MAC address:
    //Serial.println(central.address());
    readTimeCounter();
    //Serial.print("timeCounterInLoop: ");
    //Serial.println(timeCounter);
    //While the central is still connected to peripheral:
    while (central.connected()) {
      //We don't do nothing at this moment. It isn't necessary
    }
    //When the central disconnects, print it out:
    //Serial.print(F("Disconnected from central: "));
    //Serial.println(central.address());
  }
  //The central is disconnected. We increment the counter every second if the bike is cycling.
  //Is mouvement sensor activate?
  if (IMU.gyroscopeAvailable()) {
    //Read acceleration
    IMU.readGyroscope(x, y, z);
    //Serial.print("Acceleration:   ");
    //Serial.print(x);
    //Serial.print("     Old:   ");
    //Serial.println(antX);
    //Serial.println(int(x) != int(antX));
    //If actual read is direrent of old read, bike is moving
    if (int(x) != int(antX)) {
      //Put actual read as old
      antX = x;
      //Increment counter in one second
      timeCounter = timeCounter + 1;
      //Write in bluethooth module the value
      writeTimeCounterString();
      //Only test. We read the counter value from Bluetooth
      readTimeCounter();
      //We print the counter value to test that the value readed fron the phone is well readed. (The same that 
      //printed)
      //Serial.print("CounterInCharacteristic: ");
      //Serial.println(timeCounter);
      //We delay one second to next reading
    delay(1000);
    }
  }
} //end loop()
