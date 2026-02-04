#include "ComputerClub.h"
#include <iostream>

ComputerClub::ComputerClub(int numTables, Time start, Time end, int price)
    : totalTables(numTables),
      numFreeTable(numTables),
      startTime(start),
      endTime(end),
      price(price)

{
    tables.reserve(numTables);
    for (int i = 0; i < numTables; i++)
        tables.emplace_back(i + 1);
}

void ComputerClub::seatFromQueue(const Time &time, int numTable)
{
    std::string first = queueClients.front();
    queueClients.pop_front();

    std::cout << time.toString() << " 12 " << first << " " << numTable << std::endl;

    activeClients[first] = numTable;
    tables[numTable - 1].startSession(time);
    numFreeTable--;
}

void ComputerClub::come(const Time &time, const std::string &clientName)
{
    std::cout << time.toString() << " 1 " << clientName << "\n";

    if (time < startTime || time >= endTime)
    {
        std::cout << time.toString() << " 13 NotOpenYet" << "\n";
        return;
    }

    if (activeClients.find(clientName) != activeClients.end())
    {
        std::cout << time.toString() << " 13 YouShallNotPass" << "\n";
        return;
    }

    activeClients[clientName] = 0;
}

void ComputerClub::sitDown(const Time &time, const std::string &clientName, int numTable)
{
    std::cout << time.toString() << " 2 " << clientName << " " << numTable << "\n";

    auto iter = activeClients.find(clientName);

    if (iter == activeClients.end())
    {
        std::cout << time.toString() << " 13 ClientUnknown" << "\n";
        return;
    }

    if (tables[numTable - 1].getIsBusy())
    {
        std::cout << time.toString() << " 13 PlaceIsBusy" << "\n";
        return;
    }

    int currentTable = iter->second;
    if (currentTable > 0)
    {
        tables[currentTable - 1].endSession(time, price);
        numFreeTable++;
    }

    iter->second = numTable;
    tables[numTable - 1].startSession(time);
    numFreeTable--;
}

void ComputerClub::wait(const Time &time, const std::string &clientName)
{
    std::cout << time.toString() << " 3 " << clientName << "\n";

    auto iter = activeClients.find(clientName);

    if (iter == activeClients.end())
    {
        std::cout << time.toString() << " 13 ClientUnknown" << "\n";
        return;
    }

    if (numFreeTable > 0)
    {
        std::cout << time.toString() << " 13 ICanWaitNoLonger!" << "\n";
        return;
    }

    int currentTable = iter->second;
    if (currentTable > 0)
    {
        if (queueClients.size() >= static_cast<size_t>(totalTables))
        {
            std::cout << time.toString() << " 13 BigQueue" << "\n";
            return;
        }

        tables[currentTable - 1].endSession(time, price);
        iter->second = 0;
        numFreeTable++;

        if (!queueClients.empty())
        {
            seatFromQueue(time, currentTable);
        }
    }
    else
    {
        if (currentTable == 0)
        {
            for (auto queIt = queueClients.begin(); queIt != queueClients.end(); queIt++)
            {
                if (*queIt == clientName)
                {
                    std::cout << time.toString() << " 13 ClientIsAlreadyInQueue" << "\n";
                    return;
                }
            }
        }

        if (queueClients.size() >= static_cast<size_t>(totalTables))
        {
            std::cout << time.toString() << " 11 " << clientName << "\n";
            activeClients.erase(iter);
            return;
        }
    }

    queueClients.push_back(clientName);
}

void ComputerClub::leave(const Time &time, const std::string &clientName)
{
    std::cout << time.toString() << " 4 " << clientName << "\n";

    auto iter = activeClients.find(clientName);
    if (iter == activeClients.end())
    {
        std::cout << time.toString() << " 13 ClientUnknown" << std::endl;
        return;
    }

    int currentTable = iter->second;
    activeClients.erase(iter);

    if (currentTable > 0)
    {
        tables[currentTable - 1].endSession(time, price);
        numFreeTable++;

        if (!queueClients.empty())
        {
            seatFromQueue(time, currentTable);
        }
    }
    else
    {
        for (auto iter = queueClients.begin(); iter != queueClients.end();)
        {
            if (*iter == clientName)
            {
                queueClients.erase(iter);
                break;
            }
            else
                ++iter;
        }
    }
}

void ComputerClub::endDay()
{
    for (const auto &[clientName, numTable] : activeClients)
    {
        if (numTable > 0)
            tables[numTable - 1].endSession(endTime, price);

        std::cout << endTime.toString() << " 11 " << clientName << std::endl;
    }

    std::cout << endTime.toString() << std::endl;

    for (int i = 0; i < totalTables; i++)
    {
        std::cout << tables[i].getNumber() << " "
                  << tables[i].getTotalMoney() << " "
                  << Time(tables[i].getTotalTime()).toString() << std::endl;
    }
}
