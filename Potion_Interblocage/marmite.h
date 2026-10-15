#ifndef MARMITE_H
#define MARMITE_H

#include <semaphore>
// #include <QThread>

constexpr int M = 5;               // nombre de louches dans une marmite pleine
constexpr int NB_GAULOIS_MAX = 6;  // nombre maximum de buveurs

constexpr const char *NOM_VERSION = "Version AVEC INTERBLOCAGE";

struct Marmite
{
    int louches = M;               // louches restantes
    std::binary_semaphore mutex{1};           // initialise à 1 un seul Gaulois devant la marmite
    std::binary_semaphore marmiteVide{0};     // initialise à 0 la sonnette pour réveiller Asterix
    std::binary_semaphore marmitePleine{0};   // initialise à 0 Astérix crie "c'est pret !!"
};

#endif // MARMITE_H