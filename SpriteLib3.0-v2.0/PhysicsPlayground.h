#pragma once

#include "Scene.h"
#include "PhysicsPlaygroundListener.h"

class PhysicsPlayground : public Scene
{
public:
	PhysicsPlayground(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;

	void Update() override;

	//Input overrides
	void KeyboardHold() override;
	void KeyboardDown() override;
	void KeyboardUp() override;

protected:
	PhysicsPlaygroundListener listener;

	int door1CloseT = 0;
	int closeLock1 = 0;
	int closeLock2 = 0;
	int closeLock3 = 0;
	bool closeT1 = true;
	bool closeT2 = true;
	bool closeT3 = true;
	bool closeT4 = true;
	
	unsigned int rotationlid1 = 0;
	unsigned int rotationlid2 = 0;
	unsigned int rotationlid3 = 0;

	int ball = 0;
	int Tele1 = 0;
	int Tele2 = 0;
	int door2e = 0;
	int box1 = 0;
	int box2 = 0;
	int box3 = 0;
	int box4 = 0;
	int counterHolder = 0;

};
