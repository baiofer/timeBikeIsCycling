//timeBikeIsCycling

//Programm that calculates the time a bike is cycling. It gets the seconds is cycling and send this time
//when is demanded externaly. Each time the time is demanded, it goes to 0.

//Fernando Jarilla 2019

#include <Wire.h>
#include <ArduinoBLE.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Bluetooth module
BLEService bikeService("19B10000-E8F2-537E-4F6C-D104768A1214");
BLECharCharacteristic bikeCharacteristic1("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLECharCharacteristic bikeCharacteristic2("19B10002-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLECharCharacteristic bikeCharacteristic3("19B10003-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLECharCharacteristic bikeCharacteristic4("19B10004-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLECharCharacteristic bikeCharacteristic5("19B10005-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLECharCharacteristic bikeCharacteristic6("19B10006-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLECharCharacteristic bikeCharacteristic7("19B10007-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLECharCharacteristic bikeCharacteristic8("19B10008-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

//Global constants
//set counter
unsigned long timeCounter;
char timeCounterString[8];
//input for mouvement sensor
int isMoving = 4;
//value of mouvement sensor
int valueOfIsMoving = 0;

void setup(){
  //set pins as inputs
  //mouvement sensor
  pinMode(isMoving, INPUT);

  //set monitor
  Serial.begin(115200);
  while(!Serial);

  //Bluetooth module
  // begin initialization
  if (!BLE.begin()) {
    Serial.println("Failed to start BLE");
    while(1);
  }
  // set advertised local name and service UUID:
  BLE.setLocalName("BIKE");
  BLE.setAdvertisedService(bikeService);

  // add the characteristic to the service
  bikeService.addCharacteristic(bikeCharacteristic1);
  bikeService.addCharacteristic(bikeCharacteristic2);
  bikeService.addCharacteristic(bikeCharacteristic3);
  bikeService.addCharacteristic(bikeCharacteristic4);
  bikeService.addCharacteristic(bikeCharacteristic5);
  bikeService.addCharacteristic(bikeCharacteristic6);
  bikeService.addCharacteristic(bikeCharacteristic7);
  bikeService.addCharacteristic(bikeCharacteristic8);

  // add service
  BLE.addService(bikeService);

  // assing event handlers for chracteristic
  bikeCharacteristic8.setEventHandler(BLEWritten, bikeCharacteristicWritten);

  // start advertising
  BLE.advertise();

  Serial.println("Bluetooth device active. Waiting for connections ...");
  
/*
  //Interrupts
  cli();//stop interrupts

  //set timer1 interrupt at 1Hz (execute each second)
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  //set compare match register for 1hz increments
  OCR1A = 15624;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  //turn on CTC mode
  TCCR1B |= (1 << WGM12);
  //set CS12 and CS10 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  //enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  sei();//allow interrupts
*/
}//end setup()
/*
//Interrupt code
void ISR(TIMER1_COMPA_vect){//timer1 interrupt 1Hz toggles pin 13 (LED)
//generates pulse wave of frequency 1Hz/2 = 0.5kHz (takes two cycles for full wave- toggle high then toggle low)
  valueOfIsMoving = digitalRead(isMoving);
  if (!valueOfIsMoving) {
    timeCounter = timeCounter + 1;
    bikeCharacteristic.writeValue(timeCounter);
  }
} //end interrupt
*/ 

void bikeCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic) {
  // new value is wrotten to characteristic
  Serial.print("timeCounterAntes: ");
  Serial.println(timeCounter);
  readTimeCounter();
  Serial.print("timeCounterDespues: ");
  Serial.println(timeCounter);
  
}

void readTimeCounter() {
  timeCounterString[0] = bikeCharacteristic1.value();
  timeCounterString[1] = bikeCharacteristic2.value();
  timeCounterString[2] = bikeCharacteristic3.value();
  timeCounterString[3] = bikeCharacteristic4.value();
  timeCounterString[4] = bikeCharacteristic5.value();
  timeCounterString[5] = bikeCharacteristic6.value();
  timeCounterString[6] = bikeCharacteristic7.value();
  timeCounterString[7] = bikeCharacteristic8.value();
  timeCounter = atol(timeCounterString);
}

void bikeCharacteristicSubscribed(BLEDevice central, BLECharacteristic characteristic) {
  // central is connected
  //central.poll();
  Serial.print("Connected to central (with Event): ");
   // print the central's MAC address:
   Serial.println(central.address());
}

void bikeCharacteristicUnsubscribed(BLEDevice central, BLECharacteristic characteristic) {
  // central is disconnected
  Serial.print(F("Disconnected from central (with Event): "));
  Serial.println(central.address());
}

void writeTimeCounterString() {
  utoa(timeCounter, timeCounterString, 10);
  bikeCharacteristic1.writeValue(timeCounterString[0]);
  bikeCharacteristic2.writeValue(timeCounterString[1]);
  bikeCharacteristic3.writeValue(timeCounterString[2]);
  bikeCharacteristic4.writeValue(timeCounterString[3]);
  bikeCharacteristic5.writeValue(timeCounterString[4]);
  bikeCharacteristic6.writeValue(timeCounterString[5]);
  bikeCharacteristic7.writeValue(timeCounterString[6]);
  bikeCharacteristic8.writeValue(timeCounterString[7]);
}
 
void loop(){  
  // listen for BLE peripherals to connect:
  BLEDevice central = BLE.central();
  // if a central is connected to peripheral:
  if (central) {
    central.poll();
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());
    readTimeCounter();
    Serial.print("timeCounterInLoop: ");
    Serial.println(timeCounter);
    // while the central is still connected to peripheral:
    while (central.connected()) {
      // if the remote device wrote to the characteristic,
      //if (bikeCharacteristic8.valueUpdated()) {
        //bikeCharacteristic8.readValue(timeCounter);
        //Serial.print("Valor del contador leido: ");
        //Serial.println(timeCounter);
        //delay(1000);
      //}
      }
    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
  valueOfIsMoving = digitalRead(isMoving);
  //if (!valueOfIsMoving) {
    timeCounter = timeCounter + 5;
    writeTimeCounterString();
    readTimeCounter();
    Serial.print("CounterInCharacteristic: ");
    Serial.println(timeCounter);
  //}
  delay(5000);
} //end loop()
