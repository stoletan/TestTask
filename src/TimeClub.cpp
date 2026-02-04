#include "TimeClub.h"

Time::Time() : totalMinutes(0) {}
Time::Time(int minutes) : totalMinutes(minutes) {}

// From hour(XX:XX) to minute
void Time::readFromString(const std::string &s)
{
    int h = (s[0] - '0') * 10 + (s[1] - '0');
    int m = (s[3] - '0') * 10 + (s[4] - '0');

    totalMinutes = h * 60 + m;
}

// From minute to hour(XX:XX)
std::string Time::toString() const
{
    std::string s = "00:00";
    s[0] = totalMinutes / 60 / 10 + '0';
    s[1] = totalMinutes / 60 % 10 + '0';
    s[3] = totalMinutes % 60 / 10 + '0';
    s[4] = totalMinutes % 60 % 10 + '0';

    return s;
}

int Time::operator-(const Time &other) const
{
    return totalMinutes - other.totalMinutes;
}

bool Time::operator<(const Time &other) const
{
    return totalMinutes < other.totalMinutes;
}

bool Time::operator>(const Time &other) const
{
    return other < *this;
}

bool Time::operator<=(const Time &other) const
{
    return !(other < *this);
}

bool Time::operator>=(const Time &other) const
{
    return !(*this < other);
}

bool Time::operator==(const Time &other) const
{
    return totalMinutes == other.totalMinutes;
}

bool Time::operator!=(const Time &other) const
{
    return !(other == *this);
}

int Time::getTotalMinutes() const
{
    return totalMinutes;
}
