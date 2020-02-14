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
#include "utility/w5100.h"

#include "config.h"
#include "NTPServer.h"
#include "SerialDataSource.h"
#include "GPSTimeSource.h"
#include "NTPPacket.h"
#include "HTTPServer.h"
#include "TimeUtilities.h"

#include "HtmlStrings.h"

SerialDataSource dataSource;
GPSTimeSource timeSource(dataSource);
NtpServer timeServer(timeSource);

void rootPage(HttpServer *server)
{
    server->responseRedirect("/time");
}

static void zeroPrepend(HttpServer *server, int val)
{
    if (val < 10)
    {
        server->print(F("0"));
    }
    server->print(val);
}

void timePage(HttpServer *server)
{
    server->responseOK();
    uint32_t secs, fract;
    timeSource.now(&secs, &fract);
    
    uint32_t year, month, day, hour, minute, second;
    TimeUtilities::dateFromNumberOfSeconds(secs, &year, &month, &day, &hour, &minute, &second);
    
    server->print(F(COMMON_PAGE_HEADER));
    server->print(F(TIME_PAGE_HEADER));
    server->print(F(GPS_STATUS));
    if (timeSource.timeValid())
    {
        server->print(F(TIME_IS_VALID));

        server->print(year);
        server->print(F("-"));
        zeroPrepend(server, month);
        server->print(F("-"));
        zeroPrepend(server, day);
        server->print(F(" "));
        zeroPrepend(server, hour);
        server->print(F(":"));
        zeroPrepend(server, minute);
        server->print(F(":"));
        zeroPrepend(server, second);
    }
    else
    {
        server->print(F(TIME_IS_NOT_VALID));
    }

    server->print(F(TIME_PAGE_FOOTER));
    server->print(F(COMMON_PAGE_FOOTER));
}

void positionPage(HttpServer *server)
{
    server->responseOK();
    timeSource.now(NULL, NULL);
    float latitude = timeSource.latitude();
    float longitude = timeSource.longitude();
    
    server->print(F(COMMON_PAGE_HEADER));
    server->print(F(POSITION_PAGE_HEADER));
    server->print(latitude);
    server->print(F(", "));
    server->print(longitude);
    server->print(F(POSITION_PAGE_FOOTER));
    server->print(F(COMMON_PAGE_FOOTER));
}

void aboutPage(HttpServer *server)
{
    server->responseOK();
    server->print(F(ABOUT_PAGE));
}

UrlHandler handlers[] = {
    UrlHandler("/", rootPage),
    UrlHandler("/time", timePage),
    UrlHandler("/location", positionPage),
    UrlHandler("/about", aboutPage)
};

HttpServer httpServer(handlers, sizeof(handlers) / sizeof(UrlHandler));

int usingDHCP = 0;

void setup()
{
    // Print banner.
    Serial.begin(115200);
    Serial.println(F("ArduinoNTPd starting."));
    
    // Set up network.
#ifdef NETWORK_USE_DHCP
    usingDHCP = Ethernet.begin(macAddress);
    if (usingDHCP == 0)
#endif
    {
        // Use default IP if DHCP does not work or is disabled.
        Ethernet.begin(macAddress, ipAddress);
    }
    
    dataSource.begin(GPS_BAUD); // default is 4800 BAUD
    
    // NOTE: NTP server must _always_ be initialized first to ensure that it occupies socket 0
    // and thus allow input capture to work properly for grabbing RX time.
    timeServer.beginListening();
    httpServer.beginListening();

    // Enable GPS interrupts.
    timeSource.enableInterrupts();
}

void loop()
{
    bool processed = timeServer.processOneRequest();
    httpServer.processOneRequest();

#ifdef ETH_RX_PIN
    if (processed)
    {
        // Clear all interrupts.
        W5100.writeIR(0xE0);
        W5100.writeSnIR(3, 0xff);
        W5100.writeSnIR(2, 0xff);
        W5100.writeSnIR(1, 0xff);
        W5100.writeSnIR(0, 0xff);
    }
#endif

  // Renew DHCP lease if needed, etc.
  if (usingDHCP) {
    Ethernet.maintain();
  }
}

#endif // defined(ARDUINO)
