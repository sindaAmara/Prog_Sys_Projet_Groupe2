#include "asterix.h"

void Asterix::run() {
    while (true) {
        m_marmite->marmiteVide.acquire();   // dort jusqua ce qu on le reveille
        afficher("Asterix se reveille et cueille du gui...");        m_marmite->mutex.acquire();         // la ligne qui cause le probleme c'est celle la le Gaulois garde le mutex
        m_marmite->louches = M;
        pause(1500, 2500);
        afficher("La marmite est pleine ! (" + std::to_string(M) + " louches)");
        m_marmite->mutex.release();
        m_marmite->marmitePleine.release();
    }
}
