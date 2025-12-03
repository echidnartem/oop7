#include "../include/fight_visitor.h"


bool VisitorFightBandit::fight(std::shared_ptr<NPC> defender) {
    if (std::dynamic_pointer_cast<Bandit>(defender)) {
        return true;
    }
    return false;
}

bool VisitorFightBear::fight(std::shared_ptr<NPC> defender) {
    if (std::dynamic_pointer_cast<Elf>(defender)) {
        return true;
    }
    return false;
}

bool VisitorFightElf::fight(std::shared_ptr<NPC> defender) {
    if (std::dynamic_pointer_cast<Bandit>(defender)) {
        return true;
    }
    return false;
}
