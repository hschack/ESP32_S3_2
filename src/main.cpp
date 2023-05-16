#include <Arduino.h>
#include "ADS1X15.h"
#include <Wire.h>

#define I2C_SDA 4
#define I2C_SCL 5

//
//    FILE: ADS_async_differential.ino
//  AUTHOR: Rob.Tillaart
// VERSION: 0.1.0
// PURPOSE: read multiple differential continuously
//

// test
// connect 4 potmeters
//
// GND ---[   x   ]------ 5V
//            |
//
// measure at x  - connect to AIN0..4.
//
//
// choose your sensor
// ADS1013 ADS(0x48);
// ADS1014 ADS(0x48);
// ADS1015 ADS(0x48);
// ADS1113 ADS(0x48);
// ADS1114 ADS(0x48);
ADS1115 ADS(0x48);
uint8_t pair = 01;
int16_t val_01 = 0;
int16_t val_23 = 0;

bool handleConversion(void);

void setup()
{
  Serial.begin(115200);
  Serial.println(__FILE__);
  Serial.print("ADS1X15_LIB_VERSION: ");
  Serial.println(ADS1X15_LIB_VERSION);

  ADS.begin(I2C_SDA, I2C_SCL); // I2C device found at address 0x48  !
  ADS.setGain(0);        // 6.144 volt
  ADS.setDataRate(4);    // medium

  // single shot mode
  ADS.setMode(1);
  // start with first pair
  pair = 01;
  // trigger first read
  ADS.requestADC_Differential_0_1();
}


void loop()
{
  if (handleConversion() == true)
  {
    Serial.print("COMP:\t");
    Serial.print(val_23);
    Serial.println();
  }

  // do other stuff here
  delay(100);
}


// can be changed to hold other differentials reads too.
bool handleConversion()
{
  if (ADS.isReady())
  {
    if (pair == 01)
    {
      val_01 = ADS.getValue();
      pair = 23;
      ADS.requestADC_Differential_2_3();
      return false;  // only one done
    }

    // last of series to check
    if (pair == 23)
    {
      val_23 = ADS.getValue();
      pair = 01;
      ADS.requestADC_Differential_0_1();
      return true;   // both are updated
    }
  }
  return false;  // default not all read
}