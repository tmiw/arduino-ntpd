#!/bin/sh

# Run this script from the test folder to compile the test suite.
g++ -g -o PCTimeSourceTest -I../src ../src/common/*.cpp ../src/pc/*.cpp PCTimeSourceTest.cpp