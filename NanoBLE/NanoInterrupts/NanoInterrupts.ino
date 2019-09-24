#include <TimerOne.h>

//#include "CurieTimerOne.h"

int timeValue = 0;

void incTimeIsr() {
  timeValue = timeValue + 1;
  Serial.print("Value: ");
  Serial.println(timeValue);
}

void setup() {
  Serial.begin(115200);
  while(!Serial);
  
  Timer1.initialize(1000000); //1 sec in usec
  Timer1.attachInterrupt(incTimeIsr);
}

void loop() {
  //CurieTimerOne.start(timeValue, &incTimeIsr);

}
