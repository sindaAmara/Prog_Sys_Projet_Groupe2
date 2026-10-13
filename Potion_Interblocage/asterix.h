#ifndef ASTERIX_H
#define ASTERIX_H

#include "personnage.h"

// Asterix remplit la marmite quand on le reveille
class Asterix : public Personnage
{
public:
    using Personnage::Personnage;
protected:
    void run() override;
};

#endif // ASTERIX_H