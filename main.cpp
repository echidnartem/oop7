#include "include/factory.h"
#include "include/observer.h"
#include "include/battle_manager.h"

void program() {
    const int MAX_VALUE = 125; // размеры карты

    std::set<std::shared_ptr<NPC>> set_npc = generate_npc(MAX_VALUE);

    std::shared_ptr <bool> is_work_thread = std::make_shared<bool>(true);
    std::shared_ptr<std::shared_mutex> mtx_ptr = std::make_shared<std::shared_mutex>();
    std::shared_ptr<std::mutex> mtx_cout_ptr = std::make_shared<std::mutex>();

    FightFunctor fight_functor(is_work_thread, mtx_ptr, mtx_cout_ptr);
    std::shared_ptr<FightFunctor> fight_functor_ptr = std::make_shared<FightFunctor>(fight_functor);
    MoveFunctor move_functor(set_npc, fight_functor_ptr, MAX_VALUE, is_work_thread, mtx_ptr, mtx_cout_ptr);

    std::thread fight_thread(std::ref(fight_functor));
    std::thread move_thread(std::ref(move_functor));

    auto begin = std::chrono::steady_clock::now();
    auto end = std::chrono::steady_clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(end - begin);

    std::vector<std::vector<char>> grid(MAX_VALUE + 1, std::vector(MAX_VALUE + 1, '.'));
    while (elapsed_time.count() < 5) {
        grid.assign(MAX_VALUE + 1, std::vector(MAX_VALUE + 1, '.'));

        {
            std::shared_lock<std::shared_mutex> lock(*mtx_ptr);
            for (auto &npc : set_npc) {
                if (!npc->is_alive()) continue;
                const unsigned int x = npc->get_x();
                const unsigned int y = npc->get_y();
                grid[x][y] = npc->info()[1] + ('A' - 'a');
            }
        }
        {
            std::lock_guard<std::mutex> lock(*mtx_cout_ptr);
            for (int i = 0; i < grid.size(); i++) {
                for (int j = 0; j < grid[i].size(); j++) {
                    std::cout << grid[i][j];
                }
                std::cout << '\n';
            }
            std::cout << "\n\n\n";
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
        end = std::chrono::steady_clock::now();
        elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(end - begin);
    }

    *is_work_thread = false;

    move_thread.join();
    fight_thread.join();

    print_alive_npc(set_npc);
}


int main() {
    program();
    return 0;
}