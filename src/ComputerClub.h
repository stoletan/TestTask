#pragma once

#include "Table.h"
#include <map>
#include <vector>
#include <deque>
#include <string>

class ComputerClub
{
public:
    ComputerClub(int numTables, Time start, Time end, int price);

    void come(const Time &time, const std::string &clientName);
    void sitDown(const Time &time, const std::string &clientName, int numTable);
    void wait(const Time &time, const std::string &clientName);
    void leave(const Time &time, const std::string &clientName);
    void endDay();

private:
    int totalTables;
    int numFreeTable;
    Time startTime;
    Time endTime;
    int price;

    std::vector<Table> tables;
    std::map<std::string, int> activeClients; // <clientName, numTable>
    std::deque<std::string> queueClients;

    void seatFromQueue(const Time &time, int numTable);
};