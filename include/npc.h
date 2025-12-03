#pragma once


#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <algorithm>
#include <set>
#include "observer.h"


class NPC;
class Observer;


class NPC {
    friend std::istream& operator>>(std::istream& is, NPC& npc);

protected:
    unsigned int x = 0;
    unsigned int y = 0;
    std::string unique_name;
    bool alive = true;
    std::vector<std::shared_ptr<Observer>> observers;

public:
    NPC() = default;

    NPC(const unsigned int x, const unsigned int y, const std::string &name);

    NPC(NPC&& other) noexcept;
    NPC(const NPC& other) = default;

    unsigned int get_x() const;
    unsigned int get_y() const;
    std::string get_name() const;
    virtual unsigned int get_distance_step() const = 0;
    virtual unsigned int get_distance_kill() const = 0;
    bool is_alive() const;
    void make_dead();

    virtual bool accept(std::shared_ptr <NPC> npc_ptr) const = 0;

    void attach(std::shared_ptr <Observer> observer);
    void detach(std::shared_ptr <Observer> observer);
    void notify(NPC &defender);

    virtual std::string info() const = 0;
    std::ostream& print(std::ostream& os) const;

    std::ofstream& save(std::ofstream &os) const;

    void move(int shift_x, int shift_y, const int MAX_VALUE);

    virtual ~NPC() = default;
};

std::ostream &print_all_npc(std::ostream &os, std::set<std::shared_ptr<NPC>> &set_npc);