/*
 * File: GPSTimeSource.cpp
 * Description:
 *   A time source that reads the time from a NMEA-compliant GPS receiver.
 * Author: Mooneer Salem <mooneer@gmail.com>
 * License: New BSD License
 */
 
#include "GPSTimeSource.h"
#include "TimeUtilities.h"

GPSTimeSource *GPSTimeSource::Singleton_ = NULL;

void GPSTimeSource::enableInterrupts()
{
#ifdef PPS_INTERRUPT_LINE
    Singleton_ = this;
    attachInterrupt(PPS_INTERRUPT_LINE, PpsInterrupt_, RISING);
    Serial.println("interrupts enabled");
#endif // PPS_INTERRUPT_LINE
}

#ifdef PPS_INTERRUPT_LINE
void GPSTimeSource::PpsInterrupt_()
{
    noInterrupts();
    Singleton_->secondsSinceEpoch_++;
    Singleton_->fractionalSecondsSinceEpoch_ = 0;
    Singleton_->millisecondsOfLastUpdate_ = micros();
    interrupts();
}
#endif // PPS_INTERRUPT_LINE

uint32_t GPSTimeSource::getSecondsSinceEpoch(void) const
{
    return secondsSinceEpoch_;
}

uint32_t GPSTimeSource::getFractionalSecondsSinceEpoch(void) const
{
    return fractionalSecondsSinceEpoch_;
}

bool GPSTimeSource::updateTime(void)
{
    bool correctFractionalSeconds = false;

#ifdef PPS_INTERRUPT_LINE
    if (hasLocked_)
#endif
    {
        correctFractionalSeconds = true;
    }

#ifdef PPS_INTERRUPT_LINE    
    while (!hasLocked_)
#endif
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
                if (fix_age != TinyGPS::GPS_INVALID_AGE && fix_age < 5000 && year >= 2013)
                {
                    uint32_t tempSeconds = 
                        TimeUtilities::numberOfSecondsSince1900Epoch(
                            year, month, day, hour, minutes, second);
                    
                    if (tempSeconds != secondsSinceEpoch_)
                    {
                        secondsSinceEpoch_ = tempSeconds;
                        millisecondsOfLastUpdate_ = micros();
                        hasLocked_ = true;
                    }
                }
                else
                {
                    // Set time to 0 if invalid.
                    // TODO: does the interface need an accessor for "invalid time"?
                    secondsSinceEpoch_ = 0;
                    fractionalSecondsSinceEpoch_ = 0;
                    millisecondsOfLastUpdate_ = micros();
                }
            }
        }
    }
    
    if (correctFractionalSeconds)
    {
        noInterrupts();
        
        // Calculate new fractional value based on system runtime
        // since the EM-406 does not seem to return anything other than whole seconds.
        uint32_t millisecondDifference = micros() - millisecondsOfLastUpdate_;

#ifndef PPS_INTERRUPT_LINE
        secondsSinceEpoch_ += (millisecondDifference / 1000000);
#endif

        uint32_t tempFract = fractionalSecondsSinceEpoch_ + (millisecondDifference % 1000000) * (0xFFFFFFFF / 1000000);
        
        if (tempFract < fractionalSecondsSinceEpoch_)
        {
#ifndef PPS_INTERRUPT_LINE
            // overflow
            secondsSinceEpoch_++;
#else
            tempFract = 0xFFFFFFFF;
#endif
        }

        fractionalSecondsSinceEpoch_ = tempFract;
#ifndef PPS_INTERRUPT_LINE
        millisecondsOfLastUpdate_ = micros();
#endif
        interrupts();
    }

    return true;
}
