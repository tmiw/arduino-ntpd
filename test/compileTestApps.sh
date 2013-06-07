#!/bin/sh

# Run this script from the test folder to compile the test suite.
echo g++ -g -o PCTimeSourceTest -I../ArduinoNTPd -I../ArduinoNTPd/libraries ../ArduinoNTPd/libraries/TinyGPS/*.cpp ../ArduinoNTPd*.cpp PCTimeSourceTest.cpp
g++ -g -o PCTimeSourceTest -I../ArduinoNTPd -I../ArduinoNTPd/libraries ../ArduinoNTPd/libraries/TinyGPS/*.cpp ../ArduinoNTPd*.cpp PCTimeSourceTest.cpp
