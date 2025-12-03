#include "../include/elf.h"
#include "../include/fight_visitor.h"


std::string Elf::info() const {
    return "Elf(" + std::to_string(this->x) + ' ' + std::to_string(this->y) + ')' + '<' + this->unique_name + '>';
}
bool Elf::accept(std::shared_ptr<NPC> npc_ptr) const {
    VisitorFightElf visitor_fight_elf;
    return visitor_fight_elf.fight(npc_ptr);
}

unsigned int Elf::get_distance_step() const {
    return this->distance_step;
}

unsigned int Elf::get_distance_kill() const {
    return this->distance_kill;
}