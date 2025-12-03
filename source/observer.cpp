#include "../include/observer.h"


FileObserver::FileObserver(const std::string &filename) {
    log_file.open(filename, std::ios::out);
}

FileObserver::~FileObserver() {
    log_file.close();
}


void FileObserver::update(const NPC &attacker, const NPC &defender) {
    log_file.open("../log.txt", std::ios::app);

    if (log_file.is_open()) {
        log_file << "[file observer]: ";
        attacker.save(log_file);
        log_file << " kills ";
        defender.save(log_file);
        log_file << '\n';
    }
    else {
        throw std::logic_error("File could not be opened");
    }
    log_file.close();
}

void ConsoleObserver::update(const NPC &attacker, const NPC &defender) {
    std::cout << "[console observer]: ";
    attacker.print(std::cout) << ' ';
    std::cout << "kills ";
    defender.print(std::cout);
    std::cout << '\n';
}