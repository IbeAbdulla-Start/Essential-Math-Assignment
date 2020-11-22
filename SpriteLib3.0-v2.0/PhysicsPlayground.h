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
	int door2CloseT = 0;
	int destroywall1 = 0;
	int destorywall2 = 0;
	int closeLock1 = 0;
	int closeLock2 = 0;
	int closeLock3 = 0;
	int closeLock4 = 0; // this will aslo destroy wall 3
	bool closeT1 = true;
	bool closeT2 = true;
	bool closeT3 = true;
	bool closeT4 = true;
	bool closeT5 = true;
	bool closeT6 = true;
	
	unsigned int rotationlid1 = 0;
	unsigned int rotationlid2 = 0;
	unsigned int rotationlid3 = 0;
	unsigned int rotationlid4 = 0;

	int lever1 = 0;
	int lever2 = 0;
	int lever3 = 0;
	int lever4 = 0;
	int lever5 = 0;
	int lever6 = 0;
	int win = 0;
	int ball = 0;
	int Tele1 = 0;
	int Tele2 = 0;
	int door2e = 0;
	int door3e = 0;
	int enddoor1 = 0;
	int enddoor2 = 0;
	int enddoor3 = 0;
	int enddoor4 = 0;
	int enddoor5 = 0;
	int enddoor6 = 0;
	int box1 = 0;
	int counterHolder = 0;

};
