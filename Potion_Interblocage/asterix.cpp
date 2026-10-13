#include "asterix.h"

void Asterix::run()
{
    while (!isInterruptionRequested()) {

        //dormir jusqu'a ce qu'un gaulois sonne
        setEtat(Dort, "Fait la sieste");
        if (!attendre(m_marmite->marmiteVide)) break;

        //asterix veut le mutex pour remettre le compteur
        emit journal("Astérix se réveille et cueille du gui...");
        setEtat(AttendMarmite, "Attend l'accès à la marmite (mutex)");
        if (!attendre(m_marmite->mutex)) break;

        //on atteint jamais cette etape
        setEtat(Prepare, "Prépare la potion magique");
        m_marmite->louches = M;
        pause(1500, 2500);
        emit louchesChangees(M);
        emit journal(QString("La marmite est pleine ! (%1 louches)").arg(M));
        m_marmite->mutex.release();
        m_marmite->marmitePleine.release();
    }
    setEtat(Termine, "Rentré dans sa hutte");
}