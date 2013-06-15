/*
 * File: config.cpp
 * Description:
 *   Configuration for ArduinoNTPd.
 * Author: Mooneer Salem <mooneer@gmail.com>
 * License: New BSD License
 */

#include "config.h"

// The desired IP address for your time server.
// IPv4 only due to limitations of the Ethernet library.
IPAddress ipAddress(192, 168, 0, 100);

// The desired MAC address for your time server.
// This should not conflict with any other devices on your network!
byte macAddress[] = { 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff };
