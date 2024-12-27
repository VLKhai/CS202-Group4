#include "MinionFactory.h"

Minion* MinionFactory::CreateGoombas(int x, int y, bool moveDirection) {
    return new Goombas(x, y, 0, moveDirection);
}

Minion* MinionFactory::CreateKoppa(int x, int y, int minionState, bool moveDirection,int iLevelType) { 
	int tempBlock;

	switch (minionState) {
	case 0: case 3:
		tempBlock = iLevelType == 0 || iLevelType == 4 ? 7 : iLevelType == 1 ? 14 : 17;
		break;
	case 1:
		tempBlock = iLevelType == 0 || iLevelType == 4 ? 4 : iLevelType == 1 ? 12 : 15;
		break;
	case 2:
		tempBlock = iLevelType == 0 || iLevelType == 4 ? 5 : iLevelType == 1 ? 13 : 16;
		break;
	}

    return new Koppa(x, y, minionState, moveDirection, tempBlock);
}
