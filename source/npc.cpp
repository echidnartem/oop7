#include "../include/npc.h"


std::istream& operator>>(std::istream& is, NPC& npc) {
    is >> npc.x >> npc.y >> npc.unique_name;
    return is;
}

NPC::NPC(NPC&& other) noexcept {
    unique_name = std::move(other.unique_name);
    x = other.x;
    y = other.y;
}

NPC::NPC(const unsigned int x, const unsigned int y, const std::string &name) {
    static std::set<std::string> names;
    static unsigned int count = 0;

    this->x = x;
    this->y = y;
    std::string copy_name = name;
    if (names.count(name) != 0) {
        copy_name += std::to_string(count++);
    }
    this->unique_name = std::move(copy_name);
    names.insert(name);
}

unsigned int NPC::get_x() const {
    return this->x;
}

unsigned int NPC::get_y() const {
    return this->y;
}

std::string NPC::get_name() const {
    return this->unique_name;
}

bool NPC::is_alive() const {
    return this->alive == true;
}

void NPC::make_dead() {
    this->alive = false;
}

void NPC::attach(std::shared_ptr<Observer> observer) {
    observers.push_back(observer);
}

void NPC::detach(std::shared_ptr<Observer> observer) {
    observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}

void NPC::notify(NPC &defender) {
    for (auto &observer : observers) {
        observer->update(*this, defender);
    }
}

std::ofstream& NPC::save(std::ofstream &os) const {
    os << this->info();
    return os;
}

std::ostream& NPC::print(std::ostream& os) const {
    os << this->info();
    return os;
}

void NPC::move(int shift_x, int shift_y, const int MAX_VALUE) {
    if (shift_x + this->x <= MAX_VALUE && shift_x + static_cast<int>(this->x) >= 0) {
        this->x += shift_x;
    }

    if (shift_y + this->y <= MAX_VALUE && shift_y + static_cast<int>(this->y) >= 0) {
        this->y += shift_y;
    }
}

std::ostream &print_all_npc(std::ostream &os, std::set<std::shared_ptr<NPC>> &set_npc) {
    for (auto &npc : set_npc) {
        npc->print(os);
        os << "   ";
    }
    return os;
}