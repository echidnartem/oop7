#pragma once


#include <filesystem>
#include <iostream>
#include <fstream>
#include "npc.h"


class NPC;


class Observer {
public:
    virtual void update(const NPC &attacker, const NPC &defender) = 0;
    Observer() = default;
    Observer(const Observer &) = default;
    virtual ~Observer() = default;
};

class FileObserver final : public Observer {
private:
    std::ofstream log_file;
public:
    explicit FileObserver(const std::string &filename);
    void update(const NPC &attacker, const NPC &defender) override;

    ~FileObserver() override;
};

class ConsoleObserver final : public Observer {
public:
    void update(const NPC &attacker, const NPC &defender) override;
};