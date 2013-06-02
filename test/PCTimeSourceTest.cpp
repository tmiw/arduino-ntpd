/*
 * File: PCTimeSourceTestApp.cpp
 * Description:
 *   Test application for PC time source. Mainly intended to test time utility code.
 * Author: Mooneer Salem <mooneer@gmail.com>
 * License: New BSD License
 */

#include <iostream>
#include "pc/PCTimeSource.h"

int main(void)
{
    PCTimeSource timeSource;
    timeSource.updateTime();
    
    std::cout << "Current time (seconds since 1900): " << timeSource.getSecondsSinceEpoch() << std::endl;
    return 0;
}