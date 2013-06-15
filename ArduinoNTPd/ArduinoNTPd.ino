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
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include "SerialDataSource.h"
#include "GPSTimeSource.h"
#include "NTPPacket.h"

// TODO: put these in a config.h or something. Or maybe read from the EEPROM.
IPAddress ipAddress(192, 168, 0, 100);
byte macAddress[] = { 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff };
#define NTP_PORT 123

SerialDataSource dataSource;
GPSTimeSource timeSource(dataSource);
EthernetUDP ntpUdp;

void setup()
{
    // Print banner.
    Serial.begin(115200);
    Serial.println("ArduinoNTPd starting.");
    
    // Set up network.
    Ethernet.begin(macAddress, ipAddress);
    ntpUdp.begin(NTP_PORT);
    
    // Enable GPS interrupts.
    timeSource.enableInterrupts();
}

void loop()
{
    if (timeSource.updateTime())
    {
        /*Serial.print("Secs since 1900: ");
        Serial.print(timeSource.getSecondsSinceEpoch());
        Serial.print(".");
        Serial.println(timeSource.getFractionalSecondsSinceEpoch());*/
    }
    
    int packetDataSize = ntpUdp.parsePacket();
    if (packetDataSize && packetDataSize >= NtpPacket::PACKET_SIZE)
    {
        // Received what is probably an NTP packet. Read it in and verify
        // that it's legit.
        NtpPacket packet;
        ntpUdp.read((char*)&packet, NtpPacket::PACKET_SIZE);
        // TODO: verify packet.
        
        // We need the time we've received the packet in our response.
        uint32_t recvSecs = timeSource.getSecondsSinceEpoch();
        uint32_t recvFract = timeSource.getFractionalSecondsSinceEpoch();
        
        // Populate response.
        packet.swapEndian();        
        packet.leapIndicator(0);
        packet.versionNumber(4);
        packet.mode(4);
        packet.stratum = 1;
        packet.poll = 10; // 6-10 per RFC 5905.
        packet.precision = -18; // ~one microsecond precision.
        packet.rootDelay = 0; //60 * (0xFFFF / 1000); // ~60 milliseconds, TBD
        packet.rootDispersion = 0; //10 * (0xFFFF / 1000); // ~10 millisecond dispersion, TBD
        packet.referenceId[0] = 'G';
        packet.referenceId[1] = 'P';
        packet.referenceId[2] = 'S';
        packet.referenceId[3] = 0;
        packet.referenceTimestampSeconds = timeSource.getSecondsSinceEpoch();
        packet.referenceTimestampFraction = timeSource.getFractionalSecondsSinceEpoch();
        packet.originTimestampSeconds = packet.transmitTimestampSeconds;
        packet.originTimestampFraction = packet.transmitTimestampFraction;
        packet.receiveTimestampSeconds = recvSecs;
        packet.receiveTimestampFraction = recvFract;
        
        // ...and the transmit time.
        packet.transmitTimestampSeconds = timeSource.getSecondsSinceEpoch();
        packet.transmitTimestampFraction = timeSource.getFractionalSecondsSinceEpoch();
        
        // Now transmit the response to the client.
        packet.swapEndian();
        ntpUdp.beginPacket(ntpUdp.remoteIP(), ntpUdp.remotePort());
        for (int count = 0; count < NtpPacket::PACKET_SIZE; count++)
        {
            ntpUdp.write(packet.packet()[count]);
        }
        ntpUdp.endPacket();
    }
}

#endif // defined(ARDUINO)
