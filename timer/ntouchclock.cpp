#include "ntouchclock.h"

NTouchClock::NTouchClock()
{
    hour = minute = sec = centisec = 0;
}

void NTouchClock::reset()
{
    hour = minute = sec = centisec = 0;
}

std::string NTouchClock::hourstr()
{
    std::string value("");
    if (hour < 10)
    {
        value += "0";
        value += std::to_string(hour);
    }
    else
        value += std::to_string(hour%60);
    return value;
}

std::string NTouchClock::minutestr()
{
    std::string value("");
    if (minute < 10)
    {
        value += "0";
        value += std::to_string(minute);
    }
    else
        value += std::to_string(minute%60);
    return value;
}

std::string NTouchClock::secstr()
{
    std::string value("");
    if (sec < 10)
    {
        value += "0";
        value += std::to_string(sec);
    }
    else
        value += std::to_string(sec%60);
    return value;
}

std::string NTouchClock::centisecstr()
{
    std::string value("");
    if (centisec < 10)
    {
        value += "0";
        value += std::to_string(centisec);
    }
    else
        value += std::to_string(centisec);
    return value;
}

void NTouchClock::compute_time(long _usec_amount)
{
    long total_sec = (long)(_usec_amount*0.000001);
    centisec = (int) ((_usec_amount-(((long)(_usec_amount*0.000001))*1000000))*0.0001);
    sec     = (int)total_sec%60;
    minute  = (int)(total_sec/60)%60;
    hour    = (int)(total_sec/3600);
}

std::string NTouchClock::compute_timestr(long _usec_amount)
{
    std::string value;
    int h, m, s, c;
    long total_sec = (long)(_usec_amount*0.000001);

    c = (int) ((_usec_amount-(((long)(_usec_amount*0.000001))*1000000))*0.0001);
    s = (int)total_sec%60;
    m = (int)(total_sec/60)%60;
    h = (int)(total_sec/3600);

    if (h < 10)
    {
        value += "0";
        value += std::to_string(h);
    }
    else
        value += std::to_string(h%60);
    value += ":";
    if (m < 10)
    {
        value += "0";
        value += std::to_string(m);
    }
    else
        value += std::to_string(m%60);
    value += ":";
    if (s < 10)
    {
        value += "0";
        value += std::to_string(s);
    }
    else
        value += std::to_string(s%60);
    value += ".";
    if (c < 10)
    {
        value += "0";
        value += std::to_string(c);
    }
    else
        value += std::to_string(c);
    return value;
}
