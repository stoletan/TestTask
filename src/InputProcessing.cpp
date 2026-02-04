#include "InputProcessing.h"
#include "ComputerClub.h"
#include <sstream>
#include <iostream>

void reportError(const std::string &line)
{
    std::cout << line << "\n";
}

bool isValidNumber(const std::string &s)
{
    if (s.empty() || s[0] == '0')
        return false;

    for (size_t i = 0; i < s.length(); i++)
    {
        char c = s[i];

        if (c < '0' || c > '9')
            return false;
    }

    return true;
}

bool isValidTime(const std::string &s)
{
    if (s.length() != 5 || s[2] != ':')
        return false;

    for (int i : {0, 1, 3, 4})
    {
        if (s[i] < '0' || s[i] > '9')
            return false;
    }

    int h = (s[0] - '0') * 10 + (s[1] - '0');
    int m = (s[3] - '0') * 10 + (s[4] - '0');

    if (h >= 24 || m >= 60)
        return false;

    return true;
}

bool isValidName(const std::string &s)
{
    if (s.empty())
        return false;

    for (size_t i = 0; i < s.length(); i++)
    {
        char c = s[i];

        if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || c == '-' || c == '_'))
            return false;
    }

    return true;
}

bool isValidId(const std::string &s)
{
    if (s.length() != 1)
        return false;

    if (s[0] < '1' || s[0] > '4')
        return false;

    return true;
}

bool isValidSpaces(const std::string &s)
{
    if (s.empty())
        return false;

    if (s.front() == ' ' || s.back() == ' ')
        return false;

    for (size_t i = 0; i < s.length() - 1; i++)
    {
        if (s[i] == ' ' && s[i + 1] == ' ')
        {
            return false;
        }
    }

    return true;
}

int strToInt(const std::string &s)
{
    int res = 0;

    for (size_t i = 0; i < s.length(); i++)
    {
        char c = s[i];

        res = res * 10 + (c - '0');
    }

    return res;
}

std::vector<std::string> splitLine(const std::string &line)
{

    std::vector<std::string> words;
    std::string w;
    std::stringstream sin(line);

    if (!isValidSpaces(line))
        return words;

    while (sin >> w)
    {
        words.push_back(w);

        if (words.size() > 4)
        {
            words.clear();
            break;
        }
    }
    return words;
}

bool isValidEventLines(const std::vector<std::string> &eventLines, int numTable)
{
    Time lastTime;

    for (size_t i = 0; i < eventLines.size(); i++)
    {
        std::string currentLine = eventLines[i];
        std::vector<std::string> words = splitLine(currentLine);

        if (words.empty())
        {
            reportError(currentLine);
            return false;
        }

        const std::string strTime = words[0];
        const std::string &strId = words[1];
        const std::string &name = words[2];

        if (!isValidTime(strTime) || !isValidId(strId) || !isValidName(name))
        {
            reportError(currentLine);
            return false;
        }

        Time nowTime;
        nowTime.readFromString(strTime);

        if (nowTime < lastTime)
        {
            reportError(currentLine);
            return false;
        }
        lastTime = nowTime;

        if (strId == "2")
        {

            if (words.size() != 4)
            {
                reportError(currentLine);
                return false;
            }

            const std::string &strTable = words[3];
            if (!isValidNumber(strTable) || (strToInt(strTable) > numTable))
            {
                reportError(currentLine);
                return false;
            }
        }
        else if (words.size() != 3)
        {
            reportError(currentLine);
            return false;
        }
    }

    return true;
}

void runClub(const std::vector<std::string> &eventLines, int numTable, const Time &startTime, const Time &endTime, int price)
{
    ComputerClub computerClub(numTable, startTime, endTime, price);

    std::cout << startTime.toString() << "\n";

    for (size_t i = 0; i < eventLines.size(); i++)
    {
        std::string currentLine = eventLines[i];
        std::vector<std::string> words = splitLine(currentLine);

        Time time;
        time.readFromString(words[0]);
        const std::string &strId = words[1];
        const std::string &name = words[2];

        if (strId == "1")
            computerClub.come(time, name);
        else if (strId == "2")
        {
            int table = strToInt(words[3]);
            computerClub.sitDown(time, name, table);
        }
        else if (strId == "3")
            computerClub.wait(time, name);
        else if (strId == "4")
            computerClub.leave(time, name);
    }

    computerClub.endDay();
}

void runFile(std::ifstream &file)
{

    std::string line;

    // line 1
    if (!std::getline(file, line))
    {
        std::cout << "\n";
        return;
    }

    if (!isValidNumber(line))
    {
        reportError(line);
        return;
    }

    int numTable = strToInt(line);

    // line 2
    if (!std::getline(file, line))
    {
        std::cout << "\n";
        return;
    }

    Time startTime;
    Time endTime;
    std::vector<std::string> inputTime = splitLine(line);

    if ((inputTime.size() != 2) || !isValidTime(inputTime[0]) || !isValidTime(inputTime[1]))
    {
        reportError(line);
        return;
    }

    startTime.readFromString(inputTime[0]);
    endTime.readFromString(inputTime[1]);

    if ((endTime - startTime) <= 0)
    {
        reportError(line);
        return;
    }

    // line 3
    if (!std::getline(file, line))
    {
        std::cout << "\n";
        return;
    }

    if (!isValidNumber(line))
    {
        reportError(line);
        return;
    }

    int price = strToInt(line);

    // other line (event)
    std::vector<std::string> eventLines;

    while (std::getline(file, line))
    {
        if (line.empty())
        {
            std::cout << "\n";
            return;
        }
        eventLines.push_back(line);
    }

    if (!isValidEventLines(eventLines, numTable))
        return;

    runClub(eventLines, numTable, startTime, endTime, price);
}
