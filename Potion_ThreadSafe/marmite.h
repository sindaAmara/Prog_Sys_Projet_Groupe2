#ifndef MARMITE_H
#define MARMITE_H

#include <QSemaphore>
#include <QThread>

constexpr int M = 5;               // nombre de louches dans une marmite pleine
constexpr int NB_GAULOIS_MAX = 6;  // nombre maximum de buveurs

constexpr const char *NOM_VERSION = "Version THREAD-SAFE";

struct Marmite
{
    int louches = M;               // louches restantes
    QSemaphore mutex{1};           // initialise à 1 un seul Gaulois devant la marmite
    QSemaphore marmiteVide{0};     // initialise à 0 la sonnette pour réveiller Asterix
    QSemaphore marmitePleine{0};   // initialise à 0 Astérix crie "c'est pret !!"
};

inline bool attendre(QSemaphore &s)
{
    while (!s.tryAcquire(1, 100)) {
        if (QThread::currentThread()->isInterruptionRequested())
            return false;
    }
    return true;
}

#endif // MARMITE_H