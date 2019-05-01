#pragma once

#include <chrono>

// a wrapper around time related functions, so everything isnt making system calls to check
class Clock
{
    public:
        // get the current time, in milliseconds
        static double GetTime()
        {
            // get and return the current epoch time in milliseconds
            using namespace std::chrono;
            auto t = high_resolution_clock::now();
            return duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch()).count();
        }
};
