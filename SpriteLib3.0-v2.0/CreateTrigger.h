#pragma once

#include "Trigger.h"

class CreateTrigger:public Trigger
{
public:
	void OnTrigger() override;

	void OnEnter() override;
	void OnExit() override;
protected:

	bool triggered = false;
};


