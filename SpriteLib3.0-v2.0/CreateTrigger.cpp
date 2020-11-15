#include "CreateTrigger.h"
#include "ECS.h"

void CreateTrigger::OnTrigger()
{
	Trigger::OnTrigger();

	if (!triggered)
	{
	
		triggered = true;
	}
}

void CreateTrigger::OnEnter()
{
	Trigger::OnEnter();

	if (!triggered)
	{
		

		triggered = true;
	}
}

void CreateTrigger::OnExit()
{
	Trigger::OnExit();
}
