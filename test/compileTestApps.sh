#!/bin/sh

# Run this script from the test folder to compile the test suite.
echo g++ -g -o PCTimeSourceTest -I../src -I../libraries ../libraries/TinyGPS/*.cpp ../src/common/*.cpp ../src/pc/*.cpp PCTimeSourceTest.cpp
g++ -g -o PCTimeSourceTest -I../src -I../libraries ../libraries/TinyGPS/*.cpp ../src/common/*.cpp ../src/pc/*.cpp PCTimeSourceTest.cpp