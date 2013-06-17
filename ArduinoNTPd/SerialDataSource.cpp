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
#ifdef HARDWARE_SERIAL_CLASS
    return HARDWARE_SERIAL_CLASS.available();
#else
    return serialPort_.available();
#endif
}

int SerialDataSource::read()
{
#ifdef HARDWARE_SERIAL_CLASS
    return HARDWARE_SERIAL_CLASS.read();
#else
    return serialPort_.read();
#endif
}

#endif // defined(ARDUINO)
