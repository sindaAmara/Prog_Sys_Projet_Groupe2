#include "gaulois.h"

void Gaulois::run() {
    for (int i = 0; i < NB_TOURS; i++) {
        m_marmite->mutex.acquire();
        if (m_marmite->louches == 0) {
            afficher(m_nom + " trouve la marmite vide et reveille Asterix !");
            m_marmite->marmiteVide.release();
            m_marmite->marmitePleine.acquire();   // attend Asterix en gardant le mutex
            m_marmite->louches = M;               // c'est le Gaulois qui remet le compteurpuisque cest lui qui possede le mutex
        }
        m_marmite->louches--;
        afficher(m_nom + " boit une louche (reste " + std::to_string(m_marmite->louches) + ")");
        pause(200, 500);
        m_marmite->mutex.release();

        pause(1500, 4000);
    }
}