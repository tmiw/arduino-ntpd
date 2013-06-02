Arduino NTP server
================================

This is a work-in-progress that will eventually allow the Arduino to act as an NTP time server
using time data from GPS satellites.

Prerequisites
-------------------------
* Arduino board [1] -- TODO: compatibility list.
* GPS shield [2] with appropriate GPS module that outputs NMEA data [3].

User's Guide
-------------------------
Coming soon.

Licensing
-------------------------
New BSD license.

Unit testing code on PC
-------------------------
<pre>
$ cd test; sh runTestApps.sh
Current time (seconds since 1900): 3579060209
</pre>

References
-------------------------
  [1]: http://arduino.cc/                         "Arduino"
  [2]: https://www.sparkfun.com/products/10710    "GPS Shield @ SparkFun"
  [3]: https://www.sparkfun.com/products/465      "GPS module compatible with GPS Shield"