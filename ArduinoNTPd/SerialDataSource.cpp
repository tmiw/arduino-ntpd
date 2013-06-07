/*
 * File: SerialDataSource.cpp
 * Description:
 *   Data source that retrieves bytes from a serial pin using NewSoftSerial.
 * Author: Mooneer Salem <mooneer@gmail.com>
 * License: New BSD License
 */

#if defined(ARDUINO)

#include "Arduino.h"
#include <SoftwareSerial.h>
#include "SerialDataSource.h"

bool SerialDataSource::available()
{
    return serialPort_.available();
}

int SerialDataSource::read()
{
    return serialPort_.read();
}

#endif // defined(ARDUINO)
