#include "Table.h"

Table::Table(int num)
    : number(num),
      totalTime(0),
      totalMoney(0),
      isBusy(false),
      startTime(0) {}

void Table::startSession(const Time &time)
{
    startTime = time;
    isBusy = true;
}

void Table::endSession(const Time &time, int price)
{
    int differenceTime = time - startTime;

    totalTime += differenceTime;

    int h = differenceTime / 60;
    int m = differenceTime % 60;

    if (m == 0)
        totalMoney += h * price;
    else
        totalMoney += (h + 1) * price;

    isBusy = false;
}

int Table::getNumber() const
{
    return number;
}

int Table::getTotalTime() const
{
    return totalTime;
}

int Table::getTotalMoney() const
{
    return totalMoney;
}

bool Table::getIsBusy() const
{
    return isBusy;
}
