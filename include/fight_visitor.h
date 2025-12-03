#pragma once


#include <set>
#include "npc.h"
#include "bandit.h"
#include "bear.h"
#include "elf.h"


class IVisitorFight {
public:
    virtual bool fight(std::shared_ptr <NPC> defender) = 0;

    virtual ~IVisitorFight() = default;
};

struct VisitorFightBandit final : IVisitorFight  { bool fight(std::shared_ptr<NPC> defender) override; };
struct VisitorFightBear final : IVisitorFight { bool fight(std::shared_ptr<NPC> defender) override; };
struct VisitorFightElf final : IVisitorFight { bool fight(std::shared_ptr<NPC> defender) override; };