#include <gtest/gtest.h>
#include "club.h"
#include "utils.h"

class ClubTest : public ::testing::Test
{
protected:
    ClubConfig config;
    std::vector<Event> events;

    void SetUp() override
    {
        config.numTables = 1;
        config.startTime = "09:00";
        config.endTime = "21:00";
        config.costPerHour = 10;
    }
};

TEST_F(ClubTest, FirstTestCase)
{
    config.numTables = 3;
    config.startTime = "09:00";
    config.endTime = "19:00";
    config.costPerHour = 10;

    events = {
        {"08:48", 1, "client1"},
        {"09:41", 1, "client1"},
        {"09:48", 1, "client2"},
        {"09:52", 3, "client1"},
        {"09:54", 2, "client1 1"},
        {"10:25", 2, "client2 2"},
        {"10:58", 1, "client3"},
        {"10:59", 2, "client3 3"},
        {"11:30", 1, "client4"},
        {"11:35", 2, "client4 2"},
        {"11:45", 3, "client4"},
        {"12:33", 4, "client1"},
        {"12:43", 4, "client2"},
        {"15:52", 4, "client4"}};

    Club club(config, events);
    club.process();

    const auto &outputEvents = club.getOutputEvents();
    const auto &tables = club.getTables();

    std::vector<std::string> expectedEvents = {
        "08:48 1 client1",
        "08:48 13 NotOpenYet",
        "09:41 1 client1",
        "09:48 1 client2",
        "09:52 3 client1",
        "09:52 13 ICanWaitNoLonger!",
        "09:54 2 client1 1",
        "10:25 2 client2 2",
        "10:58 1 client3",
        "10:59 2 client3 3",
        "11:30 1 client4",
        "11:35 2 client4 2",
        "11:35 13 PlaceIsBusy",
        "11:45 3 client4",
        "12:33 4 client1",
        "12:33 12 client4 1",
        "12:43 4 client2",
        "15:52 4 client4",
        "19:00 11 client3"};

    ASSERT_EQ(outputEvents.size(), expectedEvents.size());
    for (size_t i = 0; i < outputEvents.size(); ++i)
    {
        EXPECT_EQ(outputEvents[i].toString(), expectedEvents[i]);
    }

    EXPECT_EQ(tables[0].getNumber(), 1);
    EXPECT_EQ(tables[0].getRevenue(), 70);
    EXPECT_EQ(tables[0].getOccupiedTime(), "05:58");

    EXPECT_EQ(tables[1].getNumber(), 2);
    EXPECT_EQ(tables[1].getRevenue(), 30);
    EXPECT_EQ(tables[1].getOccupiedTime(), "02:18");

    EXPECT_EQ(tables[2].getNumber(), 3);
    EXPECT_EQ(tables[2].getRevenue(), 90);
    EXPECT_EQ(tables[2].getOccupiedTime(), "08:01");
}

TEST_F(ClubTest, SecondTestCase)
{
    config.numTables = 2;
    config.startTime = "10:00";
    config.endTime = "22:00";
    config.costPerHour = 15;

    events = {
        {"09:59", 1, "client1"}};

    Club club(config, events);
    club.process();

    const auto &outputEvents = club.getOutputEvents();
    const auto &tables = club.getTables();

    std::vector<std::string> expectedEvents = {
        "09:59 1 client1",
        "09:59 13 NotOpenYet"};

    ASSERT_EQ(outputEvents.size(), expectedEvents.size());
    for (size_t i = 0; i < outputEvents.size(); ++i)
    {
        EXPECT_EQ(outputEvents[i].toString(), expectedEvents[i]);
    }

    EXPECT_EQ(tables[0].getRevenue(), 0);
    EXPECT_EQ(tables[0].getOccupiedTime(), "00:00");
    EXPECT_EQ(tables[1].getRevenue(), 0);
    EXPECT_EQ(tables[1].getOccupiedTime(), "00:00");
}

TEST_F(ClubTest, ThirdTestCase)
{
    config.numTables = 1;
    config.startTime = "09:00";
    config.endTime = "21:00";
    config.costPerHour = 50;

    events = {
        {"09:00", 1, "client1"},
        {"09:01", 2, "client1 1"},
        {"09:30", 4, "client1"}};

    Club club(config, events);
    club.process();

    const auto &outputEvents = club.getOutputEvents();
    const auto &tables = club.getTables();

    std::vector<std::string> expectedEvents = {
        "09:00 1 client1",
        "09:01 2 client1 1",
        "09:30 4 client1"};

    ASSERT_EQ(outputEvents.size(), expectedEvents.size());
    for (size_t i = 0; i < outputEvents.size(); ++i)
    {
        EXPECT_EQ(outputEvents[i].toString(), expectedEvents[i]);
    }

    EXPECT_EQ(tables[0].getRevenue(), 50);
    EXPECT_EQ(tables[0].getOccupiedTime(), "00:29");
}

TEST_F(ClubTest, FourthTestCase)
{
    config.numTables = 2;
    config.startTime = "08:00";
    config.endTime = "10:00";
    config.costPerHour = 20;

    events = {
        {"08:01", 1, "client1"},
        {"08:02", 2, "client1 1"},
        {"08:03", 1, "client2"},
        {"08:04", 2, "client2 2"}};

    Club club(config, events);
    club.process();

    const auto &outputEvents = club.getOutputEvents();
    const auto &tables = club.getTables();

    std::vector<std::string> expectedEvents = {
        "08:01 1 client1",
        "08:02 2 client1 1",
        "08:03 1 client2",
        "08:04 2 client2 2",
        "10:00 11 client1",
        "10:00 11 client2"};

    ASSERT_EQ(outputEvents.size(), expectedEvents.size());
    for (size_t i = 0; i < outputEvents.size(); ++i)
    {
        EXPECT_EQ(outputEvents[i].toString(), expectedEvents[i]);
    }

    EXPECT_EQ(tables[0].getRevenue(), 40);
    EXPECT_EQ(tables[0].getOccupiedTime(), "01:58");

    EXPECT_EQ(tables[1].getRevenue(), 40);
    EXPECT_EQ(tables[1].getOccupiedTime(), "01:56");
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}