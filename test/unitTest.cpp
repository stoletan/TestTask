#include <ctime>
#include <gtest/gtest.h>
#include "TimeClub.h"
#include "Table.h"
#include "InputProcessing.h"

using namespace std;

// Time
TEST(TimeSuite, Read)
{
    Time t;

    t.readFromString("00:00");
    EXPECT_EQ(t.getTotalMinutes(), 0);

    t.readFromString("23:59");
    EXPECT_EQ(t.getTotalMinutes(), 1439);

    t.readFromString("01:05");
    EXPECT_EQ(t.getTotalMinutes(), 65);
}

TEST(TimeSuite, ToString)
{
    EXPECT_EQ(Time(1439).toString(), "23:59");
    EXPECT_EQ(Time(0).toString(), "00:00");
    EXPECT_EQ(Time(65).toString(), "01:05");
}

// Table
TEST(TableSuite, Busy)
{
    Table t(1);
    int price = 100;

    t.startSession(Time(0));
    EXPECT_TRUE(t.getIsBusy());

    t.endSession(Time(1), price);
    EXPECT_FALSE(t.getIsBusy());
}

TEST(TableSuite, SeveralSession)
{
    Table t(1);
    int price = 100;

    t.startSession(Time(0));
    t.endSession(Time(1), price);
    EXPECT_EQ(t.getTotalTime(), 1);
    EXPECT_EQ(t.getTotalMoney(), 100);

    t.startSession(Time(60));
    t.endSession(Time(121), price);
    EXPECT_EQ(t.getTotalTime(), 62);
    EXPECT_EQ(t.getTotalMoney(), 300);

    t.startSession(Time(200));
    t.endSession(Time(260), price);
    EXPECT_EQ(t.getTotalTime(), 122);
    EXPECT_EQ(t.getTotalMoney(), 400);

    t.startSession(Time(300));
    t.endSession(Time(300), price);
    EXPECT_EQ(t.getTotalTime(), 122);
    EXPECT_EQ(t.getTotalMoney(), 400);
}

TEST(TableSuite, FullDaySession)
{
    Table t(1);
    int price = 100;

    t.startSession(Time(0));
    t.endSession(Time(1439), price);
    EXPECT_EQ(t.getTotalTime(), 1439);
    EXPECT_EQ(t.getTotalMoney(), 2400);
}

// Valid
TEST(ValidSuite, Number)
{
    EXPECT_TRUE(isValidNumber("123456789"));
    EXPECT_TRUE(isValidNumber("3"));

    EXPECT_FALSE(isValidNumber(""));
    EXPECT_FALSE(isValidNumber("-1"));
    EXPECT_FALSE(isValidNumber("0"));
    EXPECT_FALSE(isValidNumber("01234567"));
    EXPECT_FALSE(isValidNumber("1!"));
}

TEST(ValidSuite, TimeFormat)
{
    EXPECT_TRUE(isValidTime("00:00"));
    EXPECT_TRUE(isValidTime("12:34"));

    EXPECT_FALSE(isValidTime("0"));
    EXPECT_FALSE(isValidTime("9:10"));
    EXPECT_FALSE(isValidTime("24:00"));
    EXPECT_FALSE(isValidTime("12:60"));
    EXPECT_FALSE(isValidTime("12 20"));
    EXPECT_FALSE(isValidTime("a3:12"));
}

TEST(ValidSuite, Name)
{
    EXPECT_TRUE(isValidName("client_name"));
    EXPECT_TRUE(isValidName("client-name"));
    EXPECT_TRUE(isValidName("clientname123"));

    EXPECT_FALSE(isValidName(""));
    EXPECT_FALSE(isValidName("client name"));
    EXPECT_FALSE(isValidName("clientName"));
    EXPECT_FALSE(isValidName("имя"));
    EXPECT_FALSE(isValidName(" clientName"));
}

TEST(ValidSuite, Id)
{
    EXPECT_TRUE(isValidId("1"));
    EXPECT_TRUE(isValidId("2"));
    EXPECT_TRUE(isValidId("3"));
    EXPECT_TRUE(isValidId("4"));

    EXPECT_FALSE(isValidId(""));
    EXPECT_FALSE(isValidId("5"));
    EXPECT_FALSE(isValidId("0"));
    EXPECT_FALSE(isValidId("01"));
}

TEST(ValidSuite, Spaces)
{
    EXPECT_TRUE(isValidSpaces("hello world!"));

    EXPECT_FALSE(isValidSpaces(""));
    EXPECT_FALSE(isValidSpaces("hello world! "));
    EXPECT_FALSE(isValidSpaces(" hello world!"));
    EXPECT_FALSE(isValidSpaces("hello  world!"));
}

TEST(ConvertSuite, StrToInt)
{
    EXPECT_EQ(strToInt("123"), 123);
    EXPECT_EQ(strToInt("0"), 0);
    EXPECT_EQ(strToInt("101"), 101);
}

TEST(SplitSuite, SplitThreeWords)
{
    std::vector<std::string> words = splitLine("19:30 1 client2");
    ASSERT_EQ(words.size(), 3);
    EXPECT_EQ(words[0], "19:30");
    EXPECT_EQ(words[1], "1");
    EXPECT_EQ(words[2], "client2");
}

TEST(SplitSuite, SplitFourWords)
{
    std::vector<std::string> words = splitLine("19:30 2 client2 2");
    EXPECT_EQ(words.size(), 4);
    EXPECT_EQ(words[0], "19:30");
    EXPECT_EQ(words[1], "2");
    EXPECT_EQ(words[2], "client2");
    EXPECT_EQ(words[3], "2");
}

TEST(SplitSuite, SplitFiveWords)
{
    std::vector<std::string> words = splitLine("19:30 2 client2 2 2");
    EXPECT_EQ(words.size(), 0);
}

TEST(SplitSuite, SplitSpace)
{
    std::vector<std::string> words = splitLine("19:30  2 client2 2");
    EXPECT_EQ(words.size(), 0);
}

TEST(ValidEventLines, Chronology)
{
    std::vector<std::string> events = {
        "09:00 1 client1",
        "09:10 1 client2",
        "09:05 1 client3"};

    EXPECT_FALSE(isValidEventLines(events, 3));
}

TEST(ValidEventLines, TableCount)
{
    std::vector<std::string> events = {"10:00 2 client1 5"};
    EXPECT_FALSE(isValidEventLines(events, 3));
    EXPECT_TRUE(isValidEventLines(events, 5));
}

TEST(ValidEventLines, InputParametrs)
{
    std::vector<std::string> events = {"10:00 2 client1"};
    EXPECT_FALSE(isValidEventLines(events, 3));

    events = {"10:00 2 client1 abc"};
    EXPECT_FALSE(isValidEventLines(events, 3));

    events = {"10:00 client1 2"};
    EXPECT_FALSE(isValidEventLines(events, 3));

    events = {"10:00 1 client1 2"};
    EXPECT_FALSE(isValidEventLines(events, 3));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}