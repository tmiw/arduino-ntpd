#!/bin/sh

# Run this script from the test folder to compile the test suite.
echo g++ -g -o PCTimeSourceTest -I../ArduinoNTPd -I../pc -I../ArduinoNTPd/libraries/TinyGPS ../ArduinoNTPd/libraries/TinyGPS/*.cpp ../pc/*.cpp ../ArduinoNTPd/*.cpp PCTimeSourceTest.cpp
g++ -g -o PCTimeSourceTest -I../ArduinoNTPd -I../pc -I../ArduinoNTPd/libraries/TinyGPS ../ArduinoNTPd/libraries/TinyGPS/*.cpp ../pc/*.cpp ../ArduinoNTPd/*.cpp PCTimeSourceTest.cpp
