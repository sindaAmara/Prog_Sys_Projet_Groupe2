#ifndef MARMITE_H
#define MARMITE_H

#include <semaphore>
#include <mutex>

const int M = 5;          // nombre de louches dans une marmite pleine
const int NB_TOURS = 4;   // nombre de fois que chaque Gaulois va boire

// Ressource partagee par tous les threads
struct Marmite {
    int louches = M;                         // compteur protege par le mutex
    std::binary_semaphore mutex{1};          // un seul Gaulois a la fois devant la marmite
    std::binary_semaphore marmiteVide{0};    // un Gaulois reveille Asterix
    std::binary_semaphore marmitePleine{0};  // Asterix previent que la marmite est pleine
    std::mutex ecran;
};

#endif // MARMITE_H
