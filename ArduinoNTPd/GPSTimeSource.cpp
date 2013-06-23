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
volatile uint32_t overflows = 0;

void GPSTimeSource::enableInterrupts()
{
    Singleton_ = this;
    pinMode(PPS_PIN, INPUT);
    
    TCCR4A = 0 ;                    // Normal counting mode
    TCCR4B = B010;                  // set prescale bits
    TCCR4B |= _BV(ICES4);           // enable input capture
    TIMSK4 |= _BV(ICIE4);           // enable input capture interrupt for timer 4
    TIMSK4 |= _BV(TOIE4);           // overflow interrupt
    
    Serial.println("interrupts enabled");
}

void GPSTimeSource::PpsInterruptNew()
{
    // Get saved time value.
    uint32_t tmrVal = (overflows << 16) | ICR4;
    
    GPSTimeSource::Singleton_->microsecondsPerSecond_ = 
        (GPSTimeSource::Singleton_->microsecondsPerSecond_ + 
        (tmrVal - Singleton_->millisecondsOfLastUpdate_)) / 2;
    GPSTimeSource::Singleton_->secondsSinceEpoch_++;
    GPSTimeSource::Singleton_->fractionalSecondsSinceEpoch_ = 0;
    GPSTimeSource::Singleton_->millisecondsOfLastUpdate_ = tmrVal;
}

ISR(TIMER4_OVF_vect)
{
    ++overflows;
}

ISR(TIMER4_CAPT_vect)
{
    GPSTimeSource::PpsInterruptNew();
}

void GPSTimeSource::updateFractionalSeconds_(void)
{
    // Calculate new fractional value based on system runtime
    // since the EM-406 does not seem to return anything other than whole seconds.
    uint32_t lastTime = (overflows << 16) | TCNT4;
    uint32_t millisecondDifference = lastTime - millisecondsOfLastUpdate_;
    fractionalSecondsSinceEpoch_ = (millisecondDifference % microsecondsPerSecond_) * (0xFFFFFFFF / microsecondsPerSecond_);
}

void GPSTimeSource::now(uint32_t *secs, uint32_t *fract)
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
            gps_.get_position(&lat_, &long_);
            
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
                    hasLocked_ = true;
                }
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
    
    updateFractionalSeconds_();
    
    if (secs)
    {
        *secs = secondsSinceEpoch_;
    }
    if (fract)
    {
        *fract = fractionalSecondsSinceEpoch_;
    }
}
