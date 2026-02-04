#pragma once
#include <string>
class Time
{
public:
    Time();
    explicit Time(int minutes);

    // From hour(XX:XX) to minute
    void readFromString(const std::string &s);

    // From minute to hour(XX:XX)
    std::string toString() const;

    int operator-(const Time &other) const;

    bool operator<(const Time &other) const;
    bool operator>(const Time &other) const;
    bool operator<=(const Time &other) const;
    bool operator>=(const Time &other) const;
    bool operator==(const Time &other) const;
    bool operator!=(const Time &other) const;

    int getTotalMinutes() const;

private:
    int totalMinutes;
};