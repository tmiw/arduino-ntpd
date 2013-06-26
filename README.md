Arduino NTP server
================================

This is a work-in-progress that will eventually allow the Arduino to act as an NTP time server
using time data from GPS satellites.

Prerequisites
-------------------------
* [Arduino Mega2560 or equivalent] [1]
* [GPS shield] [2] with appropriate [GPS module] [3] that outputs NMEA data. Tested with EM-406.
* Ethernet shield.

Setup Guide
-------------------------
* (optional) Solder wire from INT jumper on bottom of Ethernet shield to pin 48. Comment out ETH\_RX\_PIN in config.h if not done.
* Solder 1PPS output from GPS to pin 49. GPS Shield currently comes with this unattached on the PCB.
* Solder GPS RX/TX pins to the appropriate Serial1-3 pins. By default, GPS RX/TX is on pins 18 and 19, respectively, but this can be changed in config.h. 
* Assemble board and shields. GPS should be on top, Ethernet in the middle and the Mega2560 on the bottom.
* Open sketch in the Arduino IDE.
* Verify that everything in ArduinoNTPd/config.cpp and config.h look okay.
* Upload sketch to board (right arrow button at the upper-left).

Licensing
-------------------------
New BSD license.

Unit testing code on PC
-------------------------
<pre>
$ cd test; sh runTestApps.sh
Current time (seconds since 1900): 3579060209
</pre>

  [1]: http://arduino.cc/                         "Arduino"
  [2]: https://www.sparkfun.com/products/10710    "GPS Shield @ SparkFun"
  [3]: https://www.sparkfun.com/products/465      "GPS module compatible with GPS Shield"