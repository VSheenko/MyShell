#include "gtest/gtest.h"
#include "../../utils/XmlConfigWorker.h"

#define APPSETTINGS_PATH "../../test/XmlConfigWorkerTest/testsettings.xml"

TEST(XmlConfigWorkerTest, GetValues) {
    XmlConfigWorker xmlConfigWorker(APPSETTINGS_PATH);

    std::vector<std::string> res = xmlConfigWorker.GetValues("settings/log_path");
    ASSERT_EQ(1, res.size());
    ASSERT_EQ("logs/", res[0]);

    res = xmlConfigWorker.GetValues("settings/commands_path");
    ASSERT_EQ(1, res.size());
    ASSERT_EQ("commands/", res[0]);

    res = xmlConfigWorker.GetValues("settings");
    ASSERT_EQ(0, res.size());

    res = xmlConfigWorker.GetValues("settings/Xren'");
    ASSERT_EQ(0, res.size());

    res = xmlConfigWorker.GetValues("test/test");
    ASSERT_EQ(0, res.size());

    res = xmlConfigWorker.GetValues("");
    ASSERT_EQ(0, res.size());

    res = xmlConfigWorker.GetValues("settings/test_1/test_2"); //не конечное значение
    ASSERT_EQ(0, res.size());

    res = xmlConfigWorker.GetValues("settings/test_1"); //не конечное значение
    ASSERT_EQ(0, res.size());

    res = xmlConfigWorker.GetValues("settings/test_1/test_2/test_1_3");
    ASSERT_EQ(2, res.size());
}