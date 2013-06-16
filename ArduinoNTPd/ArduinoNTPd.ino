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

void rootPage(HttpServer *server)
{
    server->responseOK();
    server->println("hello world!");
}

char *urls[] = {"/"};
UrlHandler handlers[] = {rootPage};

SerialDataSource dataSource;
GPSTimeSource timeSource(dataSource);
NtpServer timeServer(timeSource);
HttpServer httpServer(urls, handlers, 1);

void setup()
{
    // Print banner.
    Serial.begin(115200);
    Serial.println("ArduinoNTPd starting.");
    
    // Set up network.
    Ethernet.begin(macAddress, ipAddress);
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
