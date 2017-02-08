#ifndef NTOUCHCLOCK_H
#define NTOUCHCLOCK_H

#include <string>

class NTouchClock
{
    struct timeval tv;
    long time_amount;
public:
    NTouchClock();
    void reset();
    std::string hourstr();
    std::string minutestr();
    std::string secstr();
    std::string centisecstr();
    unsigned int getsec() const {return sec;}
    unsigned int getcentisec() const {return centisec;}
    void sethour(unsigned int _hour){hour = _hour;}
    void setminute(unsigned int _minute){minute = _minute;}
    void setsec(unsigned int _sec){sec = _sec;}
    void setcentisec(unsigned int _centisec){centisec = _centisec;}
    void compute_time(long _usec_amount);
    bool empty() const {if (hour&minute&sec&centisec) return false; return true;}
    static std::string compute_timestr(long _usec_amount);

private:
    unsigned int hour;
    unsigned int minute;
    unsigned int sec;
    unsigned int centisec;
};

#endif // NTOUCHCLOCK_H
