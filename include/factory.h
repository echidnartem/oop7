#pragma once


#include <fstream>
#include "npc.h"
#include "bandit.h"
#include "bear.h"
#include "elf.h"


enum class TypeNPC { bandit, bear, elf };


class FactoryNPC {
public:
    static std::shared_ptr<NPC> create_npc(const TypeNPC &type, const std::string &name, unsigned int x = 0, unsigned int y = 0);
    static std::shared_ptr<NPC> create_npc_from_file(const TypeNPC& type, std::ifstream& in);
};