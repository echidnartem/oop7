#include "../include/battle_manager.h"
#include "../include/fight_visitor.h"
#include "../include/factory.h"


FightFunctor::FightFunctor(std::shared_ptr<bool> is_work_thread,
        std::shared_ptr<std::shared_mutex> mtx,
        std::shared_ptr<std::mutex> mtx_cout) :
    is_work_thread(is_work_thread), mtx(mtx), mtx_cout(mtx_cout) {
    events = std::make_shared<std::queue<FightEvent>>();
}

void FightFunctor::add_event(std::shared_ptr<NPC> attacker, std::shared_ptr<NPC> defender) {
    events->emplace(attacker, defender);
}

FightFunctor::FightFunctor(const FightFunctor &other) {
    events = other.events;
    is_work_thread = other.is_work_thread;
    mtx = other.mtx;
    mtx_cout = other.mtx_cout;
}

bool FightFunctor::throw_the_dice() {
    unsigned int power_attack = (std::rand() % 6) + 1;
    unsigned int power_defender = (std::rand() % 6) + 1;

    if (power_attack > power_defender) {
        return true;
    }

    return false;
}

void FightFunctor::operator()() {
    while (*this->is_work_thread) {
        while (!events->empty()) {
            FightEvent current_event = events->front();
            events->pop();

            std::lock_guard<std::shared_mutex> lock(*mtx);
            std::shared_ptr<NPC>& attacker = current_event.attacker;
            std::shared_ptr<NPC>& defender = current_event.defender;
            if (attacker->is_alive()) {
                if (defender->is_alive()) {
                    if (attacker->accept(defender)) {
                        if (throw_the_dice()) {
                            defender->make_dead();
                            std::lock_guard<std::mutex> lock(*mtx_cout);
                            attacker->notify(*defender);
                        }
                    }
                }
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

bool is_murder_available(std::shared_ptr<NPC> attacker, std::shared_ptr<NPC> defender) {
    if (attacker == defender) {
        return false;
    }

    const unsigned int max_x = std::max(attacker->get_x(), defender->get_x());
    const unsigned int min_x = std::min(attacker->get_x(), defender->get_x());
    const unsigned int max_y = std::max(attacker->get_y(), defender->get_y());
    const unsigned int min_y = std::min(attacker->get_y(), defender->get_y());

    const unsigned int distance_in_a_square = (max_x - min_x) * (max_x - min_x) + (max_y - min_y) * (max_y - min_y);
    if (distance_in_a_square > (attacker->get_distance_kill() * attacker->get_distance_kill())) {
        return false;
    }

    return true;
}

void MoveFunctor::operator()() {
    while (*this->is_work_thread) {
        for (auto &attacker : set_npc) {
            std::lock_guard<std::shared_mutex> lock(*mtx);

            if (!attacker->is_alive()) {
                set_npc.erase(attacker);
                continue;
            }

            int is_negative_x = std::rand() % 2;
            int is_negative_y = std::rand() % 2;

            int shift_x = attacker->get_distance_step();
            int shift_y = attacker->get_distance_step();

            if (is_negative_x) shift_x = -shift_x;
            if (is_negative_y) shift_y = -shift_y;

            attacker->move(shift_x, shift_y, MAX_VALUE);

            for (auto &defender : set_npc) {
                if (!defender->is_alive()) {
                    set_npc.erase(defender);
                    continue;
                }

                if (is_murder_available(attacker, defender)) {
                    fight_functor_ptr->add_event(attacker, defender);
                }
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

std::set<std::shared_ptr<NPC>> generate_npc(const int MAX_VALUE) {
    std::set<std::shared_ptr<NPC> > set_npc;

    auto stdin_observer = std::make_shared<ConsoleObserver>();
    auto log_file_observer = std::make_shared<FileObserver>("../log.txt");

    const int N = 50;
    for (int i = 0; i < N; i++) {
        const int rand_type = std::rand() % 3;
        TypeNPC type = static_cast<TypeNPC>(rand_type);
        std::string name = "";
        switch (rand_type) {
            case 0:
                name = "bandit";
                break;
            case 1:
                name = "bear";
                break;
            case 2:
                name = "elf";
                break;
        }

        int x = std::rand() % MAX_VALUE;
        int y = std::rand() % MAX_VALUE;
        std::shared_ptr<NPC> npc = FactoryNPC::create_npc(type, name, x, y);
        npc->attach(stdin_observer);
        npc->attach(log_file_observer);
        set_npc.insert(npc);
    }

    return set_npc;
}

void print_alive_npc(const std::set<std::shared_ptr<NPC> > &set_npc) {
    std::cout << "Alive NPC:\n";
    for (auto &npc :set_npc) {
        if (npc->is_alive()) {
            npc->print(std::cout) << '\n';
        }
    }
}

