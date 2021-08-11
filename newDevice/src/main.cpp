#include <Arduino.h>

#include "network.h"
#include "status.h"
#include "storage.h"
#include "display.h"

Storage storage;
Network network;
Display display;

void setup() {
  delay(5000);

  Serial.begin(9600);

  storage.begin();

  delay(100);

  network.begin();
  display.begin();

  delay(1000);

  Serial.println("ESP32 is connected to Wi-Fi network");

  statusBegin();
  Serial.println("Status started");
  startDisplayTask();

}

void loop() {
  // put your main code here, to run repeatedly:

}