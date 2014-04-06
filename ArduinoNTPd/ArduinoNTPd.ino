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
        server->print("0");
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
    
    server->print(COMMON_PAGE_HEADER);
    server->print(TIME_PAGE_HEADER);
    server->print(year);
    server->print("-");
    zeroPrepend(server, month);
    server->print("-");
    zeroPrepend(server, day);
    server->print(" ");
    zeroPrepend(server, hour);
    server->print(":");
    zeroPrepend(server, minute);
    server->print(":");
    zeroPrepend(server, second);
    server->print(TIME_PAGE_FOOTER);
    server->print(COMMON_PAGE_FOOTER);
}

void positionPage(HttpServer *server)
{
    server->responseOK();
    timeSource.now(NULL, NULL);
    int32_t latitude = timeSource.latitude();
    int32_t longitude = timeSource.longitude();
    
    server->print(COMMON_PAGE_HEADER);
    server->print(POSITION_PAGE_HEADER);
    server->print(latitude / 1000000);
    server->print(".");
    server->print(abs(latitude % 1000000));
    server->print(", ");
    server->print(longitude / 1000000);
    server->print(".");
    server->print(abs(longitude % 1000000));
    server->print(POSITION_PAGE_FOOTER);
    server->print(COMMON_PAGE_FOOTER);
}

void aboutPage(HttpServer *server)
{
    server->responseOK();
    server->print(ABOUT_PAGE);
}

UrlHandler handlers[] = {
    UrlHandler("/", rootPage),
    UrlHandler("/time", timePage),
    UrlHandler("/location", positionPage),
    UrlHandler("/about", aboutPage)
};

HttpServer httpServer(handlers, sizeof(handlers) / sizeof(UrlHandler));

void setup()
{
    // Print banner.
    Serial.begin(115200);
    Serial.println("ArduinoNTPd starting.");
    
    // Set up network.
#ifdef NETWORK_USE_DHCP
    int ret = Ethernet.begin(macAddress);
    if (ret == 0)
#endif
    {
        // Use default IP if DHCP does not work or is disabled.
        Ethernet.begin(macAddress, ipAddress);
    }
    
    dataSource.begin();
    
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
}

#endif // defined(ARDUINO)
