/*
 * File: NTPServer.cpp
 * Description:
 *   NTP server implementation.
 * Author: Mooneer Salem <mooneer@gmail.com>
 * License: New BSD License
 */

#if defined(ARDUINO)

#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

#include "NTPPacket.h"
#include "NTPServer.h"
#include "config.h"

void NtpServer::beginListening()
{
    timeServerPort_.begin(NTP_PORT);
}

void NtpServer::processOneRequest()
{
    // We need the time we've received the packet in our response.
    uint32_t recvSecs = timeSource_.getSecondsSinceEpoch();
    uint32_t recvFract = timeSource_.getFractionalSecondsSinceEpoch();
        
    int packetDataSize = timeServerPort_.parsePacket();
    if (packetDataSize && packetDataSize >= NtpPacket::PACKET_SIZE)
    {
        // Received what is probably an NTP packet. Read it in and verify
        // that it's legit.
        NtpPacket packet;
        timeServerPort_.read((char*)&packet, NtpPacket::PACKET_SIZE);
        // TODO: verify packet.
        
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
        packet.referenceTimestampSeconds = timeSource_.getSecondsSinceEpoch();
        packet.referenceTimestampFraction = timeSource_.getFractionalSecondsSinceEpoch();
        packet.originTimestampSeconds = packet.transmitTimestampSeconds;
        packet.originTimestampFraction = packet.transmitTimestampFraction;
        packet.receiveTimestampSeconds = recvSecs;
        packet.receiveTimestampFraction = recvFract;
        
        // ...and the transmit time.
        packet.transmitTimestampSeconds = timeSource_.getSecondsSinceEpoch();
        packet.transmitTimestampFraction = timeSource_.getFractionalSecondsSinceEpoch();
        
        // Now transmit the response to the client.
        packet.swapEndian();
        timeServerPort_.beginPacket(timeServerPort_.remoteIP(), timeServerPort_.remotePort());
        for (int count = 0; count < NtpPacket::PACKET_SIZE; count++)
        {
            timeServerPort_.write(packet.packet()[count]);
        }
        timeServerPort_.endPacket();
    } 
}

#endif // defined(ARDUINO)
