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

#include "config.h"
#include "NTPServer.h"
#include "SerialDataSource.h"
#include "GPSTimeSource.h"
#include "NTPPacket.h"
#include "HTTPServer.h"

SerialDataSource dataSource;
GPSTimeSource timeSource(dataSource);
NtpServer timeServer(timeSource);

void rootPage(HttpServer *server)
{
    server->responseRedirect("/time");
}

void timePage(HttpServer *server)
{
    server->responseOK();
    server->print("Seconds since 1900: ");
    server->print(timeSource.getSecondsSinceEpoch());
    server->print("<br/>");
    server->print("Fractional seconds since 1900: ");
    server->print(timeSource.getFractionalSecondsSinceEpoch());
    server->print("<br/>");
}

UrlHandler handlers[] = {
    UrlHandler("/", rootPage),
    UrlHandler("/time", timePage),
};

HttpServer httpServer(handlers, 2);

void setup()
{
    // Print banner.
    Serial.begin(115200);
    Serial.println("ArduinoNTPd starting.");
    
    // Set up network.
    Ethernet.begin(macAddress, ipAddress);
    dataSource.begin();
    timeServer.beginListening();
    httpServer.beginListening();

    // Enable GPS interrupts.
    timeSource.enableInterrupts();
}

void loop()
{
    timeSource.updateTime();
    timeServer.processOneRequest();
    httpServer.processOneRequest();
}

#endif // defined(ARDUINO)
