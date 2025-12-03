#include "../include/bear.h"
#include "../include/fight_visitor.h"


std::string Bear::info() const {
    return "Bear(" + std::to_string(this->x) + ' ' + std::to_string(this->y) + ')' + '<' + this->unique_name + '>';
}
bool Bear::accept(std::shared_ptr<NPC>npc_ptr) const {
    VisitorFightBear visitor_fight_bear;
    return visitor_fight_bear.fight(npc_ptr);
}

unsigned int Bear::get_distance_step() const {
    return this->distance_step;
}

unsigned int Bear::get_distance_kill() const {
    return this->distance_kill;
}