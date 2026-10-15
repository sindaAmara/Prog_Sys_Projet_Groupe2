#include "asterix.h"

void Asterix::run()
{
    while (!isInterruptionRequested()) {

        //dormir jusqu'a ce qu'un gaulois sonne
        setEtat(Dort, "Fait la sieste");
        m_marmite->marmiteVide.acquire();
        // if (!attendre(m_marmite->marmiteVide)) break;

        //preparer la potion
        emit journal("Astérix se réveille et cueille du gui...");
        setEtat(Prepare, "Prépare la potion magique");
        pause(1500, 2500);

        //prevenir que c'est pret
        emit louchesChangees(M);
        emit journal(QString("La marmite est pleine ! (%1 louches)").arg(M));
        m_marmite->marmitePleine.release();
    }
    setEtat(Termine, "Rentré dans sa hutte");
}