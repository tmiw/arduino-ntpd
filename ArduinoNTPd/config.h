/*
 * File: config.h
 * Description:
 *   Configuration for ArduinoNTPd.
 * Author: Mooneer Salem <mooneer@gmail.com>
 * License: New BSD License
 */
 
#ifndef CONFIG_H
#define CONFIG_H

#if defined(ARDUINO)

#include <Ethernet.h>

// ----------------------------------------------------------------------------------
// NOTE: please see config.cpp for all config options that begin with "extern" below.
// ----------------------------------------------------------------------------------

// The desired IP address for your time server.
// IPv4 only due to limitations of the Ethernet library.
extern IPAddress ipAddress;

// The desired MAC address for your time server.
// This should not conflict with any other devices on your network!
extern byte macAddress[];

// The port to run your time server on. This should not normally
// need to be changed.
#define NTP_PORT 123

// Uncomment the below to take advantage of the PPS output of the EM-406.
// This will allow more accurate results to be sent to clients.
// See http://arduino.cc/en/Reference/AttachInterrupt for the correct value here.
#if defined(ARDUINO)
#define PPS_INTERRUPT_LINE 4
#endif

// RX pin for the GPS receiver/shield. This should support change interrupts
// per NewSoftSerial documentation.
#define GPS_RX_PIN 11

// TX pin for the GPS receiver/shield.
#define GPS_TX_PIN 12

#endif // defined(ARDUINO)

#endif // CONFIG_H
