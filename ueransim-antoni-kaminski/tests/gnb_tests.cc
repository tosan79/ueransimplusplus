#include <gtest/gtest.h>
#include <yaml.h>
#include <yaml_utils.hpp>

TEST(GnbConfig, MccValue) {
    auto config_file = YAML::LoadFile("/home/antoni/ueransim-antoni-kaminski/config/open5gs-gnb-01.yaml");
    int mcc = yaml::GetInt32(config_file, "mcc");
    EXPECT_EQ(mcc, 901);
}

TEST(GnbConfig, GnbId) {
    auto config_file = YAML::LoadFile("/home/antoni/ueransim-antoni-kaminski/config/open5gs-gnb-01.yaml");
    auto nci = yaml::GetInt64(config_file, "nci");
    ASSERT_EQ(nci, 0x10);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}