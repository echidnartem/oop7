#pragma once


#include "npc.h"


class Bear final: public NPC {
private:
    unsigned int distance_step = 5;
    unsigned int distance_kill = 10;
public:
    Bear() : NPC() {};
    Bear(const unsigned int x, const unsigned int y, const std::string &name) : NPC(x, y, name) {};

    Bear(Bear &&other) noexcept : NPC(std::move(other)) {};
    Bear(const Bear &other) = default;
    
    unsigned int get_distance_kill() const override;
    unsigned int get_distance_step() const override;

    bool accept(std::shared_ptr<NPC> npc_ptr) const override;

    std::string info() const override;
};