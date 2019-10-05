/*
 * File: SerialDataSource.h
 * Description:
 *   Data source that retrieves bytes from a serial pin using NewSoftSerial.
 * Author: Mooneer Salem <mooneer@gmail.com>
 * License: New BSD License
 */

#ifndef SERIAL_DATASOURCE_H
#define SERIAL_DATASOURCE_H

#if defined(ARDUINO)

#include "Arduino.h"
#include <SoftwareSerial.h>
#include "IDataSource.h"
#include "config.h"

class SerialDataSource : public IDataSource
{
public:
    SerialDataSource()
#ifndef HARDWARE_SERIAL_CLASS
        : serialPort_(GPS_RX_PIN, GPS_TX_PIN)
#endif
    {
        // empty
    }
    
    /*
     * Starts serial comms.
     */
    void begin(uint16_t BAUD = 4800)
    {
#ifdef HARDWARE_SERIAL_CLASS
#undef serialPort_
#define serialPort_ HARDWARE_SERIAL_CLASS
#endif
        // GPS module needs 4800 baud.
        serialPort_.begin(BAUD);

        // Disable everything but RMC
#if (GPS_MODULE == 0) // SiRF
        Serial.println(F("SiRF"));
        serialPort_.println(F("$PSRF103,00,00,01*08"));
        serialPort_.println(F("$PSRF103,01,00,01*09"));
        serialPort_.println(F("$PSRF103,02,00,01*0A"));
        serialPort_.println(F("$PSRF103,03,00,01*0B"));
        serialPort_.println(F("$PSRF103,04,00,01*0C"));
        serialPort_.println(F("$PSRF103,05,00,01*0D"));
        serialPort_.println(F("$PSRF103,08,00,01,00*2C"));

#elif (GPS_MODULE == 1) // MediaTek (MTK)
        Serial.println(F("MediaTek"));
        serialPort_.println(F("$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29"));
        // 1s NMEA update rate
        serialPort_.println(F("$PMTK220,1000*1F")); // 1 Hz
        // 1s fix interval
        serialPort_.println(F("$PMTK300,1000,0,0,0,0*1C")); // 1 Hz
        serialPort_.println(F("$PMTK285,2,100*23")); // Turn 1PPS on for 3D fix
#elif (GPS_MODULE == 2) // UBLOX NEO6MV2 GPS
        Serial.println(F("UBLOX"));
        serialPort_.println(F("$PUBX,40,GLL,0,0,0,0,0,0*5C"));
        serialPort_.println(F("$PUBX,40,VTG,0,0,0,0,0,0*5E"));
        serialPort_.println(F("$PUBX,40,GSV,0,0,0,0,0,0*59"));
        serialPort_.println(F("$PUBX,40,GGA,0,0,0,0,0,0*5A")); 
        serialPort_.println(F("$PUBX,40,GSA,0,0,0,0,0,0*4E"));
#else // SkyTraq Venus 6
        Serial.println(F("SkyTraq"));
        const uint8_t gps_RMC_only[16] = {0xA0, 0xA1, 0x00, 0x09, 0x08, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x09, 0x0D, 0x0A};
        const uint8_t gps_1PPS_on[10] = {0xA0, 0xA1, 0x00, 0x03, 0x3E, 0x01, 0x00, 0x3F, 0x0D, 0x0A};
  
        serialPort_.write(gps_RMC_only, 16);
        serialPort_.write(gps_1PPS_on, 10); // Turn 1PPS on for 3D fix
#endif
        
#ifdef HARDWARE_SERIAL_CLASS
#undef serialPort_
#endif
    }
    
    /*
     * Returns whether there is data available to be read.
     */
    virtual bool available();
    
    /*
     * Returns one character from the data source.
     */
    virtual int read();
private:
#ifndef HARDWARE_SERIAL_CLASS
    SoftwareSerial serialPort_;
#endif
};

#endif // defined(ARDUINO)

#endif // SERIAL_DATASOURCE_H
