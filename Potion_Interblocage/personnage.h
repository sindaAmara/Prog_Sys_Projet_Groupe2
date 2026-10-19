#ifndef PERSONNAGE_H
#define PERSONNAGE_H

#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <mutex>
#include <cstdlib>
#include "marmite.h"

// cest la classe de base chaque personnage est un thread
class Personnage {
public:
    Personnage(const std::string &nom, Marmite *marmite)
        : m_nom(nom), m_marmite(marmite) {}

    void start() { m_thread = std::thread(&Personnage::run, this); }  // on lance le thread
    void join()  { m_thread.join(); }                                  // on attend la fin du thread
    void detach() { m_thread.detach(); }   // on laisse le thread tourner seul

protected:
    virtual void run() = 0;   // ce que fait le thread (asterix ou gaulois)

    void pause(int minMs, int maxMs) { // c'est pour que ca soit un peu realiste sur le timing
        int duree = minMs + rand() % (maxMs - minMs);
        std::this_thread::sleep_for(std::chrono::milliseconds(duree));
    }

    void afficher(const std::string &message) {
        std::lock_guard<std::mutex> verrou(m_marmite->ecran);  // un seul thread ecrit dans la console a la fois
        std::cout << message << std::endl;
    }

    std::string m_nom;
    Marmite *m_marmite;       // adresse de la marmite partagee

private:
    std::thread m_thread;
};

#endif // PERSONNAGE_H