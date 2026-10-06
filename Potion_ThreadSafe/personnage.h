#ifndef PERSONNAGE_H
#define PERSONNAGE_H

#include <QThread>
#include <QString>
#include <QRandomGenerator>
#include "marmite.h"

// Les etats possibles d'un personnage
enum Etat {
    AttendMarmite,   // fait la queue
    AttendPotion,    // attend que la marmite soit pleine
    Boit,            // boit une louche
    Combat,          // combat les Romains
    Dort,            // Asterix attend qu'on le réveille
    Prepare,         // Asterix prepare la potion
    Termine          // le thread est arrête
};

class Personnage : public QThread
{
    Q_OBJECT
public:
    Personnage(int id, const QString &nom, Marmite *marmite, QObject *parent = nullptr)
        : QThread(parent), m_id(id), m_nom(nom), m_marmite(marmite) {}

    QString nom() const { return m_nom; }

signals:
    void etatChange(int id, int etat, const QString &texte);
    void journal(const QString &message);
    void louchesChangees(int louches);

protected:
    void pause(int minMs, int maxMs)
    {
        msleep(QRandomGenerator::global()->bounded(minMs, maxMs));
    }

    void setEtat(Etat e, const QString &texte)
    {
        emit etatChange(m_id, e, texte);
    }

    int m_id;
    QString m_nom;
    Marmite *m_marmite;
};

#endif // PERSONNAGE_H