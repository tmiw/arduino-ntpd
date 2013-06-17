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
    uint32_t currentTime = micros();
    Singleton_->microsecondsPerSecond_ = currentTime - Singleton_->millisecondsOfLastUpdate_;
    Singleton_->secondsSinceEpoch_++;
    Singleton_->fractionalSecondsSinceEpoch_ = 0;
    Singleton_->millisecondsOfLastUpdate_ = currentTime;
    interrupts();
}
#endif // PPS_INTERRUPT_LINE

void GPSTimeSource::updateFractionalSeconds_(void)
{
    // Calculate new fractional value based on system runtime
    // since the EM-406 does not seem to return anything other than whole seconds.
    uint32_t lastTime = micros();
    uint32_t millisecondDifference = lastTime - millisecondsOfLastUpdate_;
    fractionalSecondsSinceEpoch_ = (millisecondDifference % microsecondsPerSecond_) * (0xFFFFFFFF / microsecondsPerSecond_);
#ifndef PPS_INTERRUPT_LINE
    if (millisecondDifference >= microsecondsPerSecond_)
    {
        secondsSinceEpoch_ += millisecondDifference / microsecondsPerSecond_;
    }
#endif
}

void GPSTimeSource::now(uint32_t *secs, uint32_t *fract)
{
    noInterrupts();
    
#ifdef PPS_INTERRUPT_LINE    
    //while (!hasLocked_)
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
    
    updateFractionalSeconds_();
    
    if (secs)
    {
        *secs = secondsSinceEpoch_;
    }
    if (fract)
    {
        *fract = fractionalSecondsSinceEpoch_;
    }
    
    interrupts();
}
