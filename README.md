# Arduino-vedirect
Arduino library for reading ve.direct text protocol messages. Supports hardware and software serial

The `Collect()` method listens for incoming serial data and returns as soon as the checksum matches. 
This results in a wait of less than 1 second for data. Now we can call `GetValue()` to retrieve the different values we got from the collection.

Currently this is in use in a LoRa sensor connected to a smartsolar controller. 
It wakes up every 30 minutes, collects a ve.direct block, and transmits the data all in less than 5 seconds, optimizing battery life.

Based on a rewrite of the code found in [physee/Victron.Arduino-ESP8266](https://github.com/physee/Victron.Arduino-ESP8266). 
This code is more fault tollerant and can handle unexpected garbage/start/stop from the uart communication. It is also now in a library form.

### Alternatives
Looking for a Python alternative? Check out [metrafonic/asyncio-vedirect-mqtt](https://github.com/metrafonic/asyncio-vedirect-mqtt)

## Example usage
We connect to the smartsolar device via Serial1 and print the values as soon as they are collected.
```c++
#include "VEDirect.h"

VEDirect vedirect(Serial1);

void setup()
{
  Serial1.begin(19200);
}

void loop(){
  while (!vedirect.Collect()){
    Serial.println("Getting victron data timed out");
  }
  float batteryVoltage = atof(vedirect.GetValue(3));
  vedirect.PrintValues();
}
```
