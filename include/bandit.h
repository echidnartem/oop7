#pragma once


#include "npc.h"


class Bandit final: public NPC {
private:
    unsigned int distance_step = 10;
    unsigned int distance_kill = 10;
public:
    Bandit() : NPC() {};
    Bandit(const unsigned int x, const unsigned int y, const std::string &name) : NPC(x, y, name) {};

    Bandit(Bandit &&other) noexcept : NPC(std::move(other)) {};
    Bandit(const Bandit &other) = default;

    unsigned int get_distance_kill() const override;
    unsigned int get_distance_step() const override;

    bool accept(std::shared_ptr<NPC> npc_ptr) const override;

    std::string info() const override;
};