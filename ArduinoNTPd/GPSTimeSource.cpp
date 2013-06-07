/*
 * File: GPSTimeSource.cpp
 * Description:
 *   A time source that reads the time from a NMEA-compliant GPS receiver.
 * Author: Mooneer Salem <mooneer@gmail.com>
 * License: New BSD License
 */
 
#include "GPSTimeSource.h"
#include "TimeUtilities.h"

uint32_t GPSTimeSource::getSecondsSinceEpoch(void) const
{
    return secondsSinceEpoch_;
}

uint32_t GPSTimeSource::getFractionalSecondsSinceEpoch(void) const
{
    return fractionalSecondsSinceEpoch_;
}

void GPSTimeSource::updateTime(void)
{
    while (dataSource_.available())
    {
        int c = dataSource_.read();
        if (gps_.encode(c))
        {
            // Grab time from now-valid data.
            int year;
            byte month, day, hour, minutes, second, hundredths;
            unsigned long fix_age;

            gps_.crack_datetime(&year, &month, &day,
              &hour, &minutes, &second, &hundredths, &fix_age);
              
            // We don't want to use the time we've received if 
            // the fix is invalid.
            if (fix_age != TinyGPS::GPS_INVALID_AGE && fix_age < 5000)
            {
                secondsSinceEpoch_ =
                    TimeUtilities::numberOfSecondsSince1900Epoch(
                        year, month, day, hour, minutes, second);
                fractionalSecondsSinceEpoch_ =
                    ((int)hundredths * 10) * (0xFFFFFF / 1000);
            }
            else
            {
                // Set time to 0 if invalid.
                // TODO: does the interface need an accessor for "invalid time"?
                secondsSinceEpoch_ = 0;
                fractionalSecondsSinceEpoch_ = 0;
            }
        }
    }
}
