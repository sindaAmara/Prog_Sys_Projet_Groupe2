#ifndef MARMITE_H
#define MARMITE_H

// #include <QSemaphore>
// #include <QThread>

#include<semaphore>
// #include<thread>

const int M = 5;               // nombre de louches dans une marmite pleine
const int NB_GAULOIS_MAX = 6;  // nombre maximum de buveurs

constexpr const char *NOM_VERSION = "Version THREAD-SAFE";

struct Marmite
{
    int louches = M;               // louches restantes
    // On rappel que 1 = dispo, 0 = occupé
    // clé de blocage, baton de parole
    std::binary_semaphore mutex{1};           // nombre de demande possible, 1 si on peut demander 0 sinon non
    // semaphore de signal
    std::binary_semaphore marmiteVide{0};     // signal a 0 pour asterix
    std::binary_semaphore marmitePleine{0};   // signal a 0 pour dire que la marmite est prete
};

// inline bool attendre(QSemaphore &s)
// {
//     while (!s.tryAcquire(1, 100)) {
//         if (QThread::currentThread()->isInterruptionRequested())
//             return false;
//     }
//     return true;
// }

#endif // MARMITE_H