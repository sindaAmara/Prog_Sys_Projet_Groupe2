#include <QApplication>
#include <QTimer>
#include <QDebug>
#include <QList>
#include "marmite.h"
#include "asterix.h"
#include "gaulois.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //on va creer une seule marmite
    Marmite *marmite = new Marmite;

    //creer les personnages et ils reçoivent tous l'adresse de la même marmite
    QList<Personnage *> personnages;
    personnages.append(new Asterix(0, "Astérix", marmite));
    personnages.append(new Gaulois(1, "Obélix", marmite));
    personnages.append(new Gaulois(2, "Abraracourcix", marmite));
    personnages.append(new Gaulois(3, "Assurancetourix", marmite));

    //on affiche dans la console chaque message "journal"
    for (Personnage *p : personnages) {
        QObject::connect(p, &Personnage::journal, &app, [](const QString &message) {
            qInfo().noquote() << message;
        });
    }

    //lancerr tous les threads
    for (Personnage *p : personnages)
        p->start();

    //arreter le programme au bout de 20 secondes
    QTimer::singleShot(20000, &app, &QApplication::quit);
    int resultat = app.exec();

    //arreter les threads
    for (Personnage *p : personnages)
        p->requestInterruption();
    for (Personnage *p : personnages) {
        p->wait();
        delete p;
    }
    delete marmite;
    return resultat;
}