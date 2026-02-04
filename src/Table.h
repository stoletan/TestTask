#pragma once

#include "TimeClub.h"

class Table
{
public:
    explicit Table(int num);

    void startSession(const Time &time);

    // Calc time and money
    void endSession(const Time &time, int price);

    int getNumber() const;
    int getTotalTime() const;
    int getTotalMoney() const;
    bool getIsBusy() const;

private:
    int number;
    int totalTime;
    int totalMoney;
    bool isBusy;
    Time startTime;
};