#include "TranslateTrigger.h"
#include "ECS.h"

void TranslateTrigger::OnTrigger()
{
	Trigger::OnTrigger();
}

void TranslateTrigger::OnEnter()
{
	Trigger::OnEnter();

	

	if (t == 0) {
		auto& object = ECS::GetComponent<TranslateT>(MainEntities::MainPlayer());
		object.teleport1 = true;
		auto& mainplayer = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
		
		
		
	}
	else if (t == 1) {
		auto& object = ECS::GetComponent<TranslateT>(MainEntities::MainPlayer());
		object.teleport2 = true;
		auto& mainplayer = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());



	}
}

void TranslateTrigger::OnExit()
{
	Trigger::OnExit();
	

	if (t == 0) {
		auto& object = ECS::GetComponent<TranslateT>(MainEntities::MainPlayer());
		object.teleport1 = false;
		
	}
	else if (t == 1) {
		auto& object = ECS::GetComponent<TranslateT>(MainEntities::MainPlayer());
		object.teleport2= false;

	}
}
	TranslateTrigger::TranslateTrigger(int translation)
	{
		t = translation;
	}
