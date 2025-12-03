#include "../include/factory.h"


std::shared_ptr<NPC> FactoryNPC::create_npc(const TypeNPC &type, const std::string &name, unsigned int x, unsigned int y) {
    switch (type) {
        case TypeNPC::bandit:
            return std::make_shared<Bandit>(x, y, name);
        case TypeNPC::bear:
            return std::make_shared<Bear>(x, y, name);
        case TypeNPC::elf:
            return std::make_shared<Elf>(x, y, name);
    }
    return nullptr;
}


std::shared_ptr<NPC> FactoryNPC::create_npc_from_file(const TypeNPC &type, std::ifstream& in) {
    switch (type) {
        case TypeNPC::bandit:
            static Bandit dragon;

            if (in.is_open()) in >> dragon;
            else throw std::logic_error("File not found");

            return std::make_shared<Bandit>(dragon);
        case TypeNPC::bear:
            static Bear bear;

            if (in.is_open()) in >> bear;
            else throw std::logic_error("File not found");

            return std::make_shared<Bear>(bear);
        case TypeNPC::elf:
            static Elf elf;

            if (in.is_open()) in >> elf;
            else throw std::logic_error("File not found");

            return std::make_shared<Elf>(elf);
    }
    return nullptr;
}