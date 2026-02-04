#pragma once

#include "TimeClub.h"
#include <string>
#include <vector>
#include <fstream>

void reportError(const std::string &line);
bool isValidNumber(const std::string &s);
bool isValidTime(const std::string &s);
bool isValidName(const std::string &s);
bool isValidId(const std::string &s);
bool isValidSpaces(const std::string &s);
int strToInt(const std::string &s);
std::vector<std::string> splitLine(const std::string &line);

bool isValidEventLines(const std::vector<std::string> &eventLines, int numTable);
void runClub(const std::vector<std::string> &eventLines, int numTable, const Time &startTime, const Time &endTime, int price);
void runFile(std::ifstream &file);