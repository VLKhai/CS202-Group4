#include "Event.h"

Event::Event(void) {
	this->endGame = false;
    this->bState = true;
    this->stepID = 0;
}

Event::~Event(void) {

}

void Event::Normal() {
    if (bState) {
        if (vOLDDir.size() > stepID) {
            if(vOLDLength[stepID] > 0) {
                switch (vOLDLength[stepID] ) {
                    case eTOP:
                        break;
                }
            } 

        }
    }
}