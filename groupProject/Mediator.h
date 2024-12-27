#pragma once
#include "Minion.h"
#include <string>

class Mediator {
public:
	virtual void notify(Minion* pmSender, std::string sEvent) = 0;
};