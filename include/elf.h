#pragma once


#include "npc.h"


class Elf final: public NPC {
private:
    unsigned int distance_step = 10;
    unsigned int distance_kill = 50;
public:
    Elf() : NPC() {};
    Elf(const unsigned int x, const unsigned int y, const std::string &name) : NPC(x, y, name) {};

    Elf(Elf &&other) noexcept : NPC(std::move(other)) {};
    Elf(const Elf &other) = default;

    unsigned int get_distance_kill() const override;
    unsigned int get_distance_step() const override;

    bool accept(std::shared_ptr<NPC> npc_ptr) const override;

    std::string info() const override;
};