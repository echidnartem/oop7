#include <gtest/gtest.h>
#include "../include/battle_manager.h"
#include "../include/factory.h"
#include "../include/observer.h"
#include "../include/fight_visitor.h"


class RPGTest : public ::testing::Test {
protected:
    void SetUp() override {
        bandit = FactoryNPC::create_npc(TypeNPC::bandit, "Shadow", 100, 200);
        bear = FactoryNPC::create_npc(TypeNPC::bear, "Grizzly", 101, 201);
        elf = FactoryNPC::create_npc(TypeNPC::elf, "Lira", 150, 250);
    }

    void TearDown() override {
        std::vector<std::string> files = {"test_save.txt", "log.txt"};
        for (const auto& file : files) {
            if (std::filesystem::exists(file)) {
                std::filesystem::remove(file);
            }
        }
    }

    std::shared_ptr<NPC> bandit;
    std::shared_ptr<NPC> bear;
    std::shared_ptr<NPC> elf;
};

TEST_F(RPGTest, FactoryCreatesCorrectTypes) {
    ASSERT_NE(bandit, nullptr);
    ASSERT_NE(bear, nullptr);
    ASSERT_NE(elf, nullptr);

    EXPECT_TRUE(bandit->info().find("Bandit") != std::string::npos);
    EXPECT_TRUE(bear->info().find("Bear") != std::string::npos);
    EXPECT_TRUE(elf->info().find("Elf") != std::string::npos);
}

TEST_F(RPGTest, CoordinatesAreValid) {
    auto check_coords = [](const std::shared_ptr<NPC>& npc) {
        EXPECT_GE(npc->get_x(), 0u);
        EXPECT_LE(npc->get_x(), 500u);
        EXPECT_GE(npc->get_y(), 0u);
        EXPECT_LE(npc->get_y(), 500u);
    };

    check_coords(bandit);
    check_coords(bear);
    check_coords(elf);
}

TEST_F(RPGTest, ObserverPattern) {
    testing::internal::CaptureStdout();

    auto console_observer = std::make_shared<ConsoleObserver>();
    bear->attach(console_observer);

    bear->notify(*elf);

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_FALSE(output.empty());
    EXPECT_TRUE(output.find("kills") != std::string::npos);
}

TEST_F(RPGTest, ObserverDetach) {
    testing::internal::CaptureStdout();

    auto observer = std::make_shared<ConsoleObserver>();
    bear->attach(observer);

    bear->notify(*elf);
    std::string output1 = testing::internal::GetCapturedStdout();
    EXPECT_FALSE(output1.empty());

    bear->detach(observer);

    testing::internal::CaptureStdout();
    bear->notify(*elf);
    std::string output2 = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output2.empty());
}

TEST_F(RPGTest, MultipleNPCManagement) {
    std::set<std::shared_ptr<NPC>> npc_set;
    for (int i = 0; i < 3; ++i) {
        npc_set.insert(FactoryNPC::create_npc(TypeNPC::bandit, "bandit"));
        npc_set.insert(FactoryNPC::create_npc(TypeNPC::elf, "elf"));
        npc_set.insert(FactoryNPC::create_npc(TypeNPC::bear, "bear"));
    }

    EXPECT_EQ(npc_set.size(), 9);

    testing::internal::CaptureStdout();
    print_all_npc(std::cout, npc_set);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_FALSE(output.empty());
    EXPECT_TRUE(output.find("bandit") != std::string::npos);
    EXPECT_TRUE(output.find("elf") != std::string::npos);
    EXPECT_TRUE(output.find("bear") != std::string::npos);
}


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}