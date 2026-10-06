#include "gaulois.h"

void Gaulois::run()
{
    while (!isInterruptionRequested()) {

        // Faire la queue devant la marmite
        setEtat(AttendMarmite, "Fait la queue devant la marmite");
        if (!attendre(m_marmite->mutex)) break;

        // Si la marmite est vide alors réveiller Astérix et attendre
        if (m_marmite->louches == 0) {
            emit journal(m_nom + " trouve la marmite vide et réveille Astérix !");
            m_marmite->marmiteVide.release();

            setEtat(AttendPotion, "Attend qu'Astérix remplisse la marmite");
            if (!attendre(m_marmite->marmitePleine)) break;

            m_marmite->louches = M;
        }

        //Prendre une louche et boire donc une louche de moins
        m_marmite->louches--;
        emit louchesChangees(m_marmite->louches);
        setEtat(Boit, "Boit une louche de potion magique");
        emit journal(m_nom + " boit une louche (reste "
                     + QString::number(m_marmite->louches) + ")");
        pause(200, 500);

        //Laisser la place au suivant gaulois
        m_marmite->mutex.release();

        //Aller combattre sur les Romains
        setEtat(Combat, "Combats les Romains !");
        pause(1500, 4000);
    }
    setEtat(Termine, "Rentré au village");
}