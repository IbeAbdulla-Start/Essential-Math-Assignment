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
	else if (d == 3) {
		auto& object = ECS::GetComponent<OnLock>(m_triggerEntity);
		object.onlock4 = true;
	}
	else if (d == 4) {
		auto& object = ECS::GetComponent<OnLock>(m_triggerEntity);
		object.onlock5 = true;
	}
	else if (d == 5) {
		auto& object = ECS::GetComponent<OnLock>(m_triggerEntity);
		object.onlock6 = true;
	}
	else if (d == 6) {
		auto& object = ECS::GetComponent<OnLock>(m_triggerEntity);
		object.onlock7 = true;
	}
	else if (d == 7) {
		auto& object = ECS::GetComponent<OnLock>(m_triggerEntity);
		object.onlock8 = true;
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
	else if (d == 3) {
		auto& object = ECS::GetComponent<OnLock>(m_triggerEntity);
		object.onlock4 = false;
	}
	else if (d == 4) {
		auto& object = ECS::GetComponent<OnLock>(m_triggerEntity);
		object.onlock5 = false;
	}
	else if (d == 5) {
		auto& object = ECS::GetComponent<OnLock>(m_triggerEntity);
		object.onlock6 = false;
	}
	else if (d == 6) {
		auto& object = ECS::GetComponent<OnLock>(m_triggerEntity);
		object.onlock7 = false;
	}
	else if (d == 7) {
		auto& object = ECS::GetComponent<OnLock>(m_triggerEntity);
		object.onlock8 = false;
	}
}

DestroyTrigger::DestroyTrigger(int destroy)
{
	d = destroy;
}