#ifndef GAULOIS_H
#define GAULOIS_H

#include "personnage.h"

class Gaulois : public Personnage {
public:
    using Personnage::Personnage;
protected:
    void run() override;
};

#endif // GAULOIS_H
