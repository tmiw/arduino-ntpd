/*
 * File: NTPPacket.h
 * Description:
 *   NTP packet representation.
 * Author: Mooneer Salem <mooneer@gmail.com>
 * License: New BSD License
 */
 
#ifndef NTP_PACKET_H
#define NTP_PACKET_H

struct NtpPacket
{
    static const int PACKET_SIZE = 48;
    
    unsigned int leapIndicator:2;
    unsigned int versionNumber:3;
    unsigned int mode:3;
    char stratum;
    char poll;
    char precision;
    unsigned int rootDelay;
    unsigned int rootDispersion;
    char referenceId[4];
    unsigned int referenceTimestampSeconds;
    unsigned int referenceTimestampFraction;
    unsigned int originTimestampSeconds;
    unsigned int originTimestampFraction;
    unsigned int receiveTimestampSeconds;
    unsigned int receiveTimestampFraction;
    unsigned int transmitTimestampSeconds;
    unsigned int transmitTimestampFraction;
};

#endif // NTP_PACKET_H