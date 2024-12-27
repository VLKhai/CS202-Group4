#ifndef MINIONFACTORY_H
#define MINIONFACTORY_H

#include "Minion.h"
#include "Goombas.h"
#include "Koppa.h"

class MinionFactory {
public:
    static Minion* CreateGoombas(int x, int y, bool moveDirection);
    static Minion* CreateKoppa(int x, int y, int minionState, bool moveDirection, int iLevelType);
};

#endif // MINIONFACTORY_H
