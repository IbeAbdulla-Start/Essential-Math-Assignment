#include "DestroyTrigger.h"
#include "ECS.h"

void DestroyTrigger::OnTrigger()
{
	Trigger::OnTrigger();
}
	

void DestroyTrigger::OnEnter()
{
	Trigger::OnEnter();
	if (d == 0) {
		
		auto& object = ECS::GetComponent<OnLock>(m_triggerEntity);
		object.onlock1 = true;
		
	}
	else if (d == 1) {

		auto& object = ECS::GetComponent<OnLock>(m_triggerEntity);
		object.onlock2 = true;

	}
	else if (d == 2) {
		auto& object = ECS::GetComponent<OnLock>(m_triggerEntity);
		object.onlock3 = true;
	}
}

void DestroyTrigger::OnExit()
{
	Trigger::OnExit();
	if (d == 0) {

		auto& object = ECS::GetComponent<OnLock>(m_triggerEntity);
		object.onlock1 = false;

	}
	else if (d == 1) {

		auto& object = ECS::GetComponent<OnLock>(m_triggerEntity);
		object.onlock2 = false;

	}
	else if (d == 2) {
		auto& object = ECS::GetComponent<OnLock>(m_triggerEntity);
		object.onlock3 = false;
	}
}

DestroyTrigger::DestroyTrigger(int destroy)
{
	d = destroy;
}