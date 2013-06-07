/*
 * File: arduino-ntpd.ino
 * Description:
 *   Main application code.
 * Author: Mooneer Salem <mooneer@gmail.com>
 * License: New BSD License
 */

#if defined(ARDUINO)

#include "Arduino.h"
#include <TinyGPS.h>
#include <SoftwareSerial.h>
#include "SerialDataSource.h"
#include "GPSTimeSource.h"

SerialDataSource dataSource;
GPSTimeSource timeSource(dataSource);

void setup()
{
    Serial.begin(115200);
    Serial.println("ArduinoNTPd starting.");
}

void loop()
{
    timeSource.updateTime();
    Serial.print("Secs since 1900: ");
    Serial.print(timeSource.getSecondsSinceEpoch());
    Serial.print(".");
    Serial.println(timeSource.getFractionalSecondsSinceEpoch());
    delay(1000);
}

#endif // defined(ARDUINO)
