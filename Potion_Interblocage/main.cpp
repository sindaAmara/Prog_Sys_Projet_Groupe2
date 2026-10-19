#include <iostream>
#include <ctime>
#include <cstdlib>
#include "marmite.h"
#include "asterix.h"
#include "gaulois.h"

int main() {
    srand(time(nullptr));

    Marmite *marmite = new Marmite;   // une seule marmite et son adresse est partagee par tous

    Asterix asterix("Asterix", marmite);
    Gaulois obelix("Obelix", marmite);
    Gaulois abraracourcix("Abraracourcix", marmite);
    Gaulois assurancetourix("Assurancetourix", marmite);

    asterix.start();
    obelix.start();
    abraracourcix.start();
    assurancetourix.start();

    obelix.join();
    abraracourcix.join();
    assurancetourix.join();

    asterix.detach();   // Asterix dort sur marmiteVide pour toujours cest pour ca qu on ne peut pas faire join()
    return 0;
}
