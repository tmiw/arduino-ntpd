#!/bin/sh

# Run this script from the test folder to compile the test suite.
echo g++ -g -o PCTimeSourceTest -I../ -I../libraries ../libraries/TinyGPS/*.cpp ../*.cpp PCTimeSourceTest.cpp
g++ -g -o PCTimeSourceTest -I../ -I../libraries ../libraries/TinyGPS/*.cpp ../*.cpp PCTimeSourceTest.cpp
