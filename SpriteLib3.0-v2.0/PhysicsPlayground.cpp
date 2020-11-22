#include "PhysicsPlayground.h"
#include "Utilities.h"

#include <random>

PhysicsPlayground::PhysicsPlayground(std::string name)
	: Scene(name)
{
	
	m_gravity = b2Vec2(0.f, -98.f);
	m_physicsWorld->SetGravity(m_gravity);

	m_physicsWorld->SetContactListener(&listener);
}

void PhysicsPlayground::InitScene(float windowWidth, float windowHeight)
{
	//Dynamically allocates the register
	m_sceneReg = new entt::registry;

	//Attach the register
	ECS::AttachRegister(m_sceneReg);

	//Sets up aspect ratio for the camera
	float aspectRatio = windowWidth / windowHeight;

	//Setup MainCamera Entity
	{
		/*Scene::CreateCamera(m_sceneReg, vec4(-75.f, 75.f, -75.f, 75.f), -100.f, 100.f, windowWidth, windowHeight, true, true);*/

		//Creates Camera entity
		auto entity = ECS::CreateEntity();
		ECS::SetIsMainCamera(entity, true);

		//Creates new orthographic camera
		ECS::AttachComponent<Camera>(entity);
		ECS::AttachComponent<HorizontalScroll>(entity);
		ECS::AttachComponent<VerticalScroll>(entity);

		vec4 temp = vec4(-75.f, 75.f, -75.f, 75.f);
		ECS::GetComponent<Camera>(entity).SetOrthoSize(temp);
		ECS::GetComponent<Camera>(entity).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		ECS::GetComponent<Camera>(entity).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, -100.f, 100.f);

		//Attaches the camera to vert and horiz scrolls
		ECS::GetComponent<HorizontalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		ECS::GetComponent<VerticalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
	}

	

	//Link entity
	{
		/*Scene::CreatePhysicsSprite(m_sceneReg, "LinkStandby", 80, 60, 1.f, vec3(0.f, 30.f, 2.f), b2_dynamicBody, 0.f, 0.f, true, true)*/

		auto entity = ECS::CreateEntity();
		ECS::SetIsMainPlayer(entity, true);

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<CanJump>(entity);
		ECS::AttachComponent<TranslateT>(entity);
		ECS::AttachComponent<OnTeleporter>(entity);

		//Sets up the components
		std::string fileName = "LinkStandby.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 40, 30);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 30.f, 2.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(2010.f), float32(10.f));  //-30,10 is spawn //2500, 315 is end condition

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		//tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);
		tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight() - shrinkY)/2.f), vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);

		tempPhsBody.SetRotationAngleDeg(0.f);
		tempPhsBody.SetFixedRotation(true);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
		tempPhsBody.SetGravityScale(1.f);

		
	}

	//controls sprite
	{
		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "controls.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 122, 90);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1);
		ECS::GetComponent<Transform>(entity).SetPosition(270, 75, 1);
	}

	//controls2 sprite
	{
		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "controls1.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 122, 90);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1);
		ECS::GetComponent<Transform>(entity).SetPosition(1425, 65, 1);
	}
	//controls3 sprite
	{
		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "controls2.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 122, 90);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1);
		ECS::GetComponent<Transform>(entity).SetPosition(2500, 65, 1);
	}

	//background sprite
	{
		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "background.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 2965, 495);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.3);
		ECS::GetComponent<Transform>(entity).SetPosition(1500, 10, 0);
	}

	//win sprite
	{
		auto entity = ECS::CreateEntity();
		win = entity;

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "win.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 131, 101);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0);
		ECS::GetComponent<Transform>(entity).SetPosition(2500, 300, 100);
	}

	//lever for first level
	{
		auto entity = ECS::CreateEntity();
		lever1 = entity;

		//Add components 
		ECS::AttachComponent<Sprite>(lever1);
		ECS::AttachComponent<Transform>(lever1);

		std::string fileName = "leverdown.png";
		ECS::GetComponent<Sprite>(lever1).LoadSprite(fileName, 20, 30);
		ECS::GetComponent<Sprite>(lever1).SetTransparency(1);
		ECS::GetComponent<Transform>(lever1).SetPosition(vec3(200.f, 230.f, 3));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);

	}

	{
	auto entity = ECS::CreateEntity();
	lever2 = entity;

	//Add components 
	ECS::AttachComponent<Sprite>(lever2);
	ECS::AttachComponent<Transform>(lever2);

	std::string fileName = "leverup.png";
	ECS::GetComponent<Sprite>(lever2).LoadSprite(fileName, 20, 30);
	ECS::GetComponent<Sprite>(lever2).SetTransparency(0);
	ECS::GetComponent<Transform>(lever2).SetPosition(vec3(200.f, 230.f, 3));

	auto& tempSpr = ECS::GetComponent<Sprite>(entity);

}
	

	{
		auto entity = ECS::CreateEntity();
		lever3 = entity;

		//Add components 
		ECS::AttachComponent<Sprite>(lever3);
		ECS::AttachComponent<Transform>(lever3);

		std::string fileName = "leverleft.png";
		ECS::GetComponent<Sprite>(lever3).LoadSprite(fileName, 50, 30);
		ECS::GetComponent<Sprite>(lever3).SetTransparency(1);
		ECS::GetComponent<Transform>(lever3).SetPosition(vec3(2500.f, 275.f, 3));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	}

	{
		auto entity = ECS::CreateEntity();
		lever4 = entity;

		//Add components 
		ECS::AttachComponent<Sprite>(lever4);
		ECS::AttachComponent<Transform>(lever4);

		std::string fileName = "leverright.png";
		ECS::GetComponent<Sprite>(lever4).LoadSprite(fileName, 50, 30);
		ECS::GetComponent<Sprite>(lever4).SetTransparency(0);
		ECS::GetComponent<Transform>(lever4).SetPosition(vec3(2500.f, 275.f, 3));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	}

	//Barrier for box and button
	Scene::CreatePhysiscsSprite(true, false, true, "boxSprite", 90, 10, 1, 2935.f, 40.f, 90, 1, 0.3, 0.3);
	{
		auto entity = ECS::CreateEntity();
		lever5 = entity;

		//Add components 
		ECS::AttachComponent<Sprite>(lever5);
		ECS::AttachComponent<Transform>(lever5);

		std::string fileName = "leverupsideleft.png";
		ECS::GetComponent<Sprite>(lever5).LoadSprite(fileName, 30, 30);
		ECS::GetComponent<Sprite>(lever5).SetTransparency(1);
		ECS::GetComponent<Transform>(lever5).SetPosition(vec3(2955.f, 10.f, 3));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	}

	{
		auto entity = ECS::CreateEntity();
		lever6 = entity;

		//Add components 
		ECS::AttachComponent<Sprite>(lever6);
		ECS::AttachComponent<Transform>(lever6);

		std::string fileName = "leverupsideright.png";
		ECS::GetComponent<Sprite>(lever6).LoadSprite(fileName, 30, 30);
		ECS::GetComponent<Sprite>(lever6).SetTransparency(0);
		ECS::GetComponent<Transform>(lever6).SetPosition(vec3(2955.f, 10.f, 3));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	}

	
	//Door 2 that will be destroyed
	{
		auto entity = ECS::CreateEntity();
		door2e = entity;

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "door.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 60, 10);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(10.f, 10.f, 1));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;

		tempDef.position.Set(float32(694), float32(25));



		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | TRIGGER, 0.3, 0.3);



		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.f));
		tempPhsBody.SetRotationAngleDeg(90.f);

	}

	//enddoor1 1
	{
		auto entity = ECS::CreateEntity();
		enddoor1 = entity;

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "door.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 100, 5);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(10.f, 10.f, 1));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;

		tempDef.position.Set(float32(2450), float32(360));



		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | TRIGGER, 0.3, 0.3);



		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.f));
		tempPhsBody.SetRotationAngleDeg(90.f);

	}
	
	//enddoor1 2
	{
		auto entity = ECS::CreateEntity();
		enddoor2 = entity;

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "door.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 100, 5);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(10.f, 10.f, 1));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;

		tempDef.position.Set(float32(2548), float32(360));



		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | TRIGGER, 0.3, 0.3);



		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.f));
		tempPhsBody.SetRotationAngleDeg(90.f);

	}

	//enddoor2 1
	{
		auto entity = ECS::CreateEntity();
		enddoor3 = entity;

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "door.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 100, 5);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(10.f, 10.f, 1));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;

		tempDef.position.Set(float32(2470), float32(360));



		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | TRIGGER, 0.3, 0.3);



		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.f));
		tempPhsBody.SetRotationAngleDeg(90.f);

	}
	//enddoor2 2
	{
		auto entity = ECS::CreateEntity();
		enddoor4 = entity;

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "door.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 100, 5);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(10.f, 10.f, 1));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;

		tempDef.position.Set(float32(2528), float32(360));



		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | TRIGGER, 0.3, 0.3);



		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.f));
		tempPhsBody.SetRotationAngleDeg(90.f);

	}

	//enddoor3 1
	{
		auto entity = ECS::CreateEntity();
		enddoor5 = entity;

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "door.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 100, 5);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(10.f, 10.f, 1));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;

		tempDef.position.Set(float32(2490), float32(360));



		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | TRIGGER, 0.3, 0.3);



		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.f));
		tempPhsBody.SetRotationAngleDeg(90.f);

	}
	//enddoor3 2
	{
		auto entity = ECS::CreateEntity();
		enddoor6 = entity;

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "door.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 100, 5);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(10.f, 10.f, 1));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;

		tempDef.position.Set(float32(2508), float32(360));



		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | TRIGGER, 0.3, 0.3);



		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.f));
		tempPhsBody.SetRotationAngleDeg(90.f);

	}

	{
		//enddoor1 trigger

		auto entity = ECS::CreateEntity();
		destroywall1 = entity;
		//Add components
		ECS::AttachComponent<Sprite>(destroywall1);
		ECS::AttachComponent<Transform>(destroywall1);
		ECS::AttachComponent<PhysicsBody>(destroywall1);
		ECS::AttachComponent<OnLock>(destroywall1);
		ECS::AttachComponent<Trigger*>(destroywall1);

		//Sets up components
		std::string fileName = "boxSprite.jpg";
		ECS::GetComponent<Sprite>(destroywall1).LoadSprite(fileName, 20, 20);
		ECS::GetComponent<Sprite>(destroywall1).SetTransparency(0);
		ECS::GetComponent<Transform>(destroywall1).SetPosition(vec3(30.f, -20.f, 3.f));
		ECS::GetComponent<Trigger*>(destroywall1) = new DestroyTrigger(5);
		ECS::GetComponent<Trigger*>(destroywall1)->SetTriggerEntity(destroywall1);
		ECS::GetComponent<Trigger*>(destroywall1)->AddTargetEntity(enddoor1);
		ECS::GetComponent<Trigger*>(destroywall1)->AddTargetEntity(enddoor2);

		auto& tempSpr = ECS::GetComponent<Sprite>(destroywall1);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(destroywall1);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(2955.f), float32(10.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
	}

	{
		//enddoor2 trigger

		auto entity = ECS::CreateEntity();
		destorywall2 = entity;
		//Add components
		ECS::AttachComponent<Sprite>(destorywall2);
		ECS::AttachComponent<Transform>(destorywall2);
		ECS::AttachComponent<PhysicsBody>(destorywall2);
		ECS::AttachComponent<OnLock>(destorywall2);
		ECS::AttachComponent<Trigger*>(destorywall2);

		//Sets up components
		std::string fileName = "boxSprite.jpg";
		ECS::GetComponent<Sprite>(destorywall2).LoadSprite(fileName, 50, 20);
		ECS::GetComponent<Sprite>(destorywall2).SetTransparency(0);
		ECS::GetComponent<Transform>(destorywall2).SetPosition(vec3(30.f, -20.f, 3.f));
		ECS::GetComponent<Trigger*>(destorywall2) = new DestroyTrigger(6);
		ECS::GetComponent<Trigger*>(destorywall2)->SetTriggerEntity(destorywall2);
		ECS::GetComponent<Trigger*>(destroywall1)->AddTargetEntity(enddoor3);
		ECS::GetComponent<Trigger*>(destroywall1)->AddTargetEntity(enddoor4);

		auto& tempSpr = ECS::GetComponent<Sprite>(destorywall2);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(destorywall2);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(2500.f), float32(275.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
	}

	//Trigger to destroy door 2
	{
	//Creates entity
	auto entity = ECS::CreateEntity();
	door2CloseT = entity;
	//Add components
	ECS::AttachComponent<Sprite>(door2CloseT);
	ECS::AttachComponent<Transform>(door2CloseT);
	ECS::AttachComponent<PhysicsBody>(door2CloseT);
	ECS::AttachComponent<OnLock>(door2CloseT);
	ECS::AttachComponent<Trigger*>(door2CloseT);

	//Sets up components
	std::string fileName = "boxSprite.jpg";
	ECS::GetComponent<Sprite>(door2CloseT).LoadSprite(fileName, 10, 10);
	ECS::GetComponent<Sprite>(door2CloseT).SetTransparency(0);
	ECS::GetComponent<Transform>(door2CloseT).SetPosition(vec3(30.f, -20.f, 3.f));
	ECS::GetComponent<Trigger*>(door2CloseT) = new DestroyTrigger(4);
	ECS::GetComponent<Trigger*>(door2CloseT)->SetTriggerEntity(door2CloseT);
	ECS::GetComponent<Trigger*>(door2CloseT)->AddTargetEntity(door2e);

	auto& tempSpr = ECS::GetComponent<Sprite>(door2CloseT);
	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(door2CloseT);

	float shrinkX = 0.f;
	float shrinkY = 0.f;
	b2Body* tempBody;
	b2BodyDef tempDef;
	tempDef.type = b2_staticBody;
	tempDef.position.Set(float32(200.f), float32(230.f));

	tempBody = m_physicsWorld->CreateBody(&tempDef);

	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
	tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
	}
	
	

	{
		auto entity = ECS::CreateEntity();
		box1 = entity;
	
		//Add components 
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components 
		std::string fileName = "boxSprite.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 100, 5);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(10.f, 10.f, 1));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
	    tempDef.type = b2_staticBody;
		
		tempDef.position.Set(float32(180), float32(50));



		tempBody = m_physicsWorld->CreateBody(&tempDef);
		
			tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | TRIGGER, 0.3, 0.3);
		
		
		
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.f));
		tempPhsBody.SetRotationAngleDeg(0.f);
	
	}

	
	//Trigger for first wall
	{
	//Creates entity
	auto entity = ECS::CreateEntity();
	door1CloseT = entity;
	//Add components
	ECS::AttachComponent<Sprite>(door1CloseT);
	ECS::AttachComponent<Transform>(door1CloseT);
	ECS::AttachComponent<PhysicsBody>(door1CloseT);
	ECS::AttachComponent<OnLock>(door1CloseT);
	ECS::AttachComponent<Trigger*>(door1CloseT);

	//Sets up components
	std::string fileName = "boxSprite.jpg";
	ECS::GetComponent<Sprite>(door1CloseT).LoadSprite(fileName, 10, 10);
	ECS::GetComponent<Sprite>(door1CloseT).SetTransparency(0);
	ECS::GetComponent<Transform>(door1CloseT).SetPosition(vec3(30.f, -20.f, 3.f));
	ECS::GetComponent<Trigger*>(door1CloseT) = new DestroyTrigger(0);
	ECS::GetComponent<Trigger*>(door1CloseT)->SetTriggerEntity(door1CloseT);
	ECS::GetComponent<Trigger*>(door1CloseT)->AddTargetEntity(box1);

	auto& tempSpr = ECS::GetComponent<Sprite>(door1CloseT);
	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(door1CloseT);

	float shrinkX = 0.f;
	float shrinkY = 0.f;
	b2Body* tempBody;
	b2BodyDef tempDef;
	tempDef.type = b2_staticBody;
	tempDef.position.Set(float32(205.f), float32(30.f));

	tempBody = m_physicsWorld->CreateBody(&tempDef);

	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
	tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
	}

	//Trigger for Lock 4

	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		closeLock4 = entity;
		//Add components
		ECS::AttachComponent<Sprite>(closeLock4);
		ECS::AttachComponent<Transform>(closeLock4);
		ECS::AttachComponent<PhysicsBody>(closeLock4);
		ECS::AttachComponent<OnLock>(closeLock4);
		ECS::AttachComponent<Trigger*>(closeLock4);

		//Sets up components
		std::string fileName = "boxSprite.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 30);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 3.f));
		ECS::GetComponent<Trigger*>(entity) = new DestroyTrigger(7);
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(closeLock4);
		ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(enddoor5);
		ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(enddoor6);

		auto& tempSpr = ECS::GetComponent<Sprite>(closeLock4);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(closeLock4);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(2950.f), float32(200.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, OBJECTS);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
	}
	
	//Trigger for Lock 3

	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		closeLock3 = entity;
		//Add components
		ECS::AttachComponent<Sprite>(closeLock3);
		ECS::AttachComponent<Transform>(closeLock3);
		ECS::AttachComponent<PhysicsBody>(closeLock3);
		ECS::AttachComponent<OnLock>(closeLock3);
		ECS::AttachComponent<Trigger*>(closeLock3);

		//Sets up components
		std::string fileName = "boxSprite.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 100, 10);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 3.f));
		ECS::GetComponent<Trigger*>(entity) = new DestroyTrigger(1);
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(closeLock3);
		ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(door3e);

		auto& tempSpr = ECS::GetComponent<Sprite>(closeLock3);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(closeLock3);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(1925.f), float32(170.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, OBJECTS);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
	}
	//Trigger for Lock 2

	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		closeLock2 = entity;
		//Add components
		ECS::AttachComponent<Sprite>(closeLock2);
		ECS::AttachComponent<Transform>(closeLock2);
		ECS::AttachComponent<PhysicsBody>(closeLock2);
		ECS::AttachComponent<OnLock>(closeLock2);
		ECS::AttachComponent<Trigger*>(closeLock2);

		//Sets up components
		std::string fileName = "boxSprite.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 90, 10);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 3.f));
		ECS::GetComponent<Trigger*>(entity) = new DestroyTrigger(2);
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(closeLock2);
		ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(door3e);

		auto& tempSpr = ECS::GetComponent<Sprite>(closeLock2);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(closeLock2);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(745.f), float32(285.f)); // position

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, OBJECTS);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
	}
	
	//Trigger for Lock 1

	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		closeLock1 = entity;
		//Add components
		ECS::AttachComponent<Sprite>(closeLock1);
		ECS::AttachComponent<Transform>(closeLock1);
		ECS::AttachComponent<PhysicsBody>(closeLock1);
		ECS::AttachComponent<OnLock>(closeLock1);
		ECS::AttachComponent<Trigger*>(closeLock1);

		//Sets up components
		std::string fileName = "boxSprite.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 90, 10);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 3.f));
		ECS::GetComponent<Trigger*>(entity) = new DestroyTrigger(3);
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(closeLock1);
		ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(door3e);

		auto& tempSpr = ECS::GetComponent<Sprite>(closeLock1);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(closeLock1);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(1425.f), float32(-20.f)); // position

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, OBJECTS);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
	}


	//Door 3 that will be destoryed
	{
		auto entity = ECS::CreateEntity();
		door3e = entity;

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "door.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 80, 10);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(10.f, 10.f, 1));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;

		tempDef.position.Set(float32(2000), float32(25));



		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | TRIGGER, 0.3, 0.3);



		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.f));
		tempPhsBody.SetRotationAngleDeg(90.f);

	}

	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		Tele1 = entity;

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<Trigger*>(entity);

		//Sets up components
		std::string fileName = "boxSprite.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 10, 10);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 3.f));
		ECS::GetComponent<Trigger*>(entity) = new TranslateTrigger(0);
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
		ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(MainEntities::MainPlayer());

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(520.f), float32(0.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
	}
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		Tele2 = entity;

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<Trigger*>(entity);

		//Sets up components
		std::string fileName = "boxSprite.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 10, 10);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 3.f));
		ECS::GetComponent<Trigger*>(entity) = new TranslateTrigger(1);
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
		ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(MainEntities::MainPlayer());

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(440.f), float32(160.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
	}
	

	//Wall0
	Scene::CreatePhysiscsSprite(true, false, true, "door.png", 100, 10, 1, -200.f, 40.f, 90, 1, 0.3, 0.3);

	//Floor1
	Scene::CreatePhysiscsSprite(true, false, true, "floor.png", 340, 15, 1, 30.f, -10.f, 0, 1, 0.3, 0.3);

	//Wall
	Scene::CreatePhysiscsSprite(true, true, false, "Wall.png", 300, 1000, 100, -300.f, 250.f, 90, 1, 0.3, 0.3);

	//Door1
	Scene::CreatePhysiscsSprite(false, false, true, "door.png", 100, 10, 1, 155.f, 100.f, 90, 1, 0.3, 0.3);

	//Floor Down 2
	Scene::CreatePhysiscsSprite(true, false, true, "floor.png", 900, 15, 1, 250.f, -10.f, 0, 1, 0.3, 0.3);
	
	//First Ramp
	Scene::CreatePhysiscsSprite(true, false, true, "box.png", 40, 48, 1, 370.f, 12.f, 0, 1, 0.3, 0.3); 

	//Teleporter Down
	Scene::CreatePhysiscsSprite(true, false, true, "teleporter.png", 50, 5, 1, 520.f, 0.f, 0, 1, 0.3, 0.3);

	//Crate 1
	Scene::CreatePhysiscsSprite(false, false, true, "boxSprite.jpg", 25, 25, 1, 580.f, 12.f, 0, 1, 0.3, 0.3);

	//Floor Up 1
	Scene::CreatePhysiscsSprite(true, false, true, "floor.png", 100, 15, 1, 410.f, 150.f, 0, 1, 0.3, 0.3);
	
	//Teleporter Up
	Scene::CreatePhysiscsSprite(true, false, true, "teleporter.png", 40, 5, 1, 440.f, 160.f, 0, 1, 0.3, 0.3);

	//Second Ramp
	Scene::CreatePhysiscsSprite(true, false, true, "floor.png", 100, 15, 1, 330.f, 175.f, -30, 1, 0.3, 0.3);

	//Floor Up 2
	Scene::CreatePhysiscsSprite(true, false, true, "floor.png", 100, 15, 1, 250.f, 200.f, 0, 1, 0.3, 0.3);

	//Wall for lever
	Scene::CreatePhysiscsSprite(true, false, true, "boxSprite.jpg", 50, 5, 1, 198.f, 230.f, 90, 1, 0.3, 0.3);
	
	

	//Second door
	Scene::CreatePhysiscsSprite(true, false, true, "boxSprite.jpg", 80, 10, 1, 694.f, 100.f, 90, 1, 0.3, 0.3); // make duplicate with y = 25.f later for closed door or dynamic body trick, either make a duplicate and make it a trigger to destroy it, then trigger another to show up after they pass the door
	
	//Wall 2
	Scene::CreatePhysiscsSprite(true, false, true, "Wall.png", 400, 50, 1, 694.f, 250.f, 90, 1, 0.3, 0.3);

	//Floor 3
	Scene::CreatePhysiscsSprite(true, false, true, "floor.png", 900, 15, 1, 950.f, -10.f, 0, 1, 0.3, 0.3);

	//Floor 4
	Scene::CreatePhysiscsSprite(true, false, true, "floor.png", 600, 15, 1, 1750.f, -10.f, 0, 1, 0.3, 0.3);

	//Lock1 
	Scene::CreatePhysiscsSprite(true, false, true, "floor.png", 55, 48, 1, 1425.f, -40.f, 0, 1, 0.3, 0.3);
	//rotationlid1 here

	rotationlid1 = Scene::CreatePhysicsSpritetoControl(true, false, true, "box.png", 20, 55, 1, 1455.f, 50.f, 90, 1, 0.3, 0.3);//Up
	 
	//Lock2
	Scene::CreatePhysiscsSprite(true, false, true, "box.png", 150, 25, 1, 745.f, 250.f, 0, 1, 0.3, 0.3); //Down
	Scene::CreatePhysiscsSprite(true, false, true, "box.png", 150, 25, 1, 745.f, 310.f, 0, 1, 0.3, 0.3); //Up
	//rotationlid2 here
	rotationlid2 = Scene::CreatePhysicsSpritetoControl(true, false, true, "box.png", 70, 25, 5, 795.f, 345.f, 0, 1, 0.3, 0.3);//Up

	//Lock3
	Scene::CreatePhysiscsSprite(true, false, true, "box.png", 100, 25, 1, 1925.f, 150.f, 0, 1, 0.3, 0.3); //Down
	Scene::CreatePhysiscsSprite(true, false, true, "box.png", 70, 25, 1, 1890.f, 175.f, 90, 1, 0.3, 0.3);
	rotationlid3 = Scene::CreatePhysicsSpritetoControl(true, false, true, "box.png", 70, 25, 5, 1970.f, 230.f, 90, 1, 0.3, 0.3);//Up

	//Wall and tile for big ball
	Scene::CreatePhysiscsSprite(true, false, true, "Wall.png", 100, 25, 1, 2100.f, 80.f, 90, 1, 0.3, 0.3); //wall
	Scene::CreatePhysiscsSprite(true, false, true, "Wall.png", 100, 25, 1, 2050.f, 120.f, 0, 1, 0.3, 0.3); //ceiling


	//Wall 3
	Scene::CreatePhysiscsSprite(true, false, true, "Wall.png", 400, 50, 1, 2000.f, 250.f, 90, 1, 0.3, 0.3);

	

	//Floor 5
	Scene::CreatePhysiscsSprite(true, false, true, "floor.png", 1000, 15, 1, 2500.f, -10.f, 0, 1, 0.3, 0.3);


	//Wall and tile for lock and barrier for box
	Scene::CreatePhysiscsSprite(true, false, true, "Wall.png", 100, 25, 1, 2888.f, 65.f, 90, 1, 0.3, 0.3); //wall right
	Scene::CreatePhysiscsSprite(true, false, true, "Wall.png", 150, 25, 1, 2950.f, 120.f, 0, 1, 0.3, 0.3); //ceilinga
	Scene::CreatePhysiscsSprite(true, false, true, "Wall.png", 150, 25, 1, 2950.f, 250.f, 0, 1, 0.3, 0.3);
	rotationlid4 = Scene::CreatePhysicsSpritetoControl(true, false, true, "box.png", 150, 25, 5, 2950.f, 250.f, 0, 1, 0.3, 0.3);//Up

	//Barrier for box and button
	Scene::CreatePhysiscsSprite(true, false, true, "boxSprite", 90, 10, 1, 2935.f, 40.f, 90, 1, 0.3, 0.3);

	
	//Wall 4
	Scene::CreatePhysiscsSprite(true, false, true, "Wall.png", 500, 50, 1, 3000.f, 225.f, 90, 1, 0.3, 0.3);

	//End Floor
	Scene::CreatePhysiscsSprite(true, false, true, "floor.png", 100, 25, 1, 2500.f, 300.f, 0, 1, 0.3, 0.3);



	//Roof
	Scene::CreatePhysiscsSprite(true, false, true, "Wall.png", 3000, 50, 1, 1500.f, 425.f, 0, 1, 0.3, 0.3);
	

	/*
	//Setup static Top Platform
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "boxSprite.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 150, 10);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(30.f), float32(-10.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), 
						float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

	}
	*/


	//Ball
	{
		auto entity = ECS::CreateEntity();
		ball = entity;
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up the components
		std::string fileName = "Ball.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 20, 20);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(45.f, -8.f, 3.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(696.f), float32(-8.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		//tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false);
		tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetWidth() - shrinkY) / 2.f), vec2(0.f, 0.f), false, OBJECTS, GROUND | ENVIRONMENT | PLAYER | TRIGGER, 0.3f);

		tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
	}
	//Ball2
	{
		auto entity = ECS::CreateEntity();
		ball = entity;
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up the components
		std::string fileName = "Ball.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 20, 20);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(45.f, -8.f, 3.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(1950.f), float32(-8.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		//tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false);
		tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetWidth() - shrinkY) / 2.f), vec2(0.f, 0.f), false, OBJECTS, GROUND | ENVIRONMENT | PLAYER | TRIGGER, 0.3f);

		tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
	}
	//Ball3
	{
		auto entity = ECS::CreateEntity();
		ball = entity;
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up the components
		std::string fileName = "Ball.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 20, 20);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(45.f, -8.f, 3.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(1925.f), float32(-8.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		//tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false);
		tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetWidth() - shrinkY) / 2.f), vec2(0.f, 0.f), false, OBJECTS, GROUND | ENVIRONMENT | PLAYER | TRIGGER, 0.3f);

		tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
	}
	

	//Ball4
	{
		auto entity = ECS::CreateEntity();
		ball = entity;
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up the components
		std::string fileName = "Ball.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 60, 60);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(45.f, -8.f, 3.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(2050.f), float32(125.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		//tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false);
		tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetWidth() - shrinkY) / 2.f), vec2(0.f, 0.f), false, OBJECTS, GROUND | ENVIRONMENT | PLAYER | TRIGGER, 0.3f);

		tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
	}
	/*
	//Setup trigger
	{
		
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<Trigger*>(entity);
		
		//Sets up components
		std::string fileName = "boxSprite.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 40, 40);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 80.f));
		ECS::GetComponent<Trigger*>(entity) = new DestroyTrigger();
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
		ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(ball);

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(300.f), float32(-30.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
	}
	*/
	

	
	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
}

void PhysicsPlayground::Update()
{
	vec3 position = ECS::GetComponent<Transform>(MainEntities::MainPlayer()).GetPosition();
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	auto& door1 = ECS::GetComponent<OnLock>(door1CloseT);
	auto& roationlocklid4 = ECS::GetComponent<PhysicsBody>(rotationlid4);
	auto& roationlocklid3 = ECS::GetComponent<PhysicsBody>(rotationlid3);
	auto& roationlocklid3sptire = ECS::GetComponent<Sprite>(rotationlid3);
	auto& roationlocklid2 = ECS::GetComponent<PhysicsBody>(rotationlid2);
	auto& roationlocklid2sptire = ECS::GetComponent<Sprite>(rotationlid2);
	auto& roationlocklid1 = ECS::GetComponent<PhysicsBody>(rotationlid1);
	auto& roationlocklid1sptire = ECS::GetComponent<Sprite>(rotationlid1);
	auto& winconditon = ECS::GetComponent<Sprite>(win);
	auto& lock4 = ECS::GetComponent<OnLock>(closeLock4);
	auto& lock3 = ECS::GetComponent<OnLock>(closeLock3);
	auto& lock2 = ECS::GetComponent<OnLock>(closeLock2);
	auto& lock1 = ECS::GetComponent<OnLock>(closeLock1);


	if (door1.onlock1) {
		if (closeT1) {
			PhysicsBody::m_bodiesToDelete.push_back(box1);
			closeT1 = false;
		}
	}

	if (lock3.onlock2 && lock2.onlock3 && lock1.onlock4) {
		if (closeT3) {
			PhysicsBody::m_bodiesToDelete.push_back(door3e);
			closeT3 = false;
		}
	}

	//lock4
	if (lock4.onlock8) {
		roationlocklid4.SetRotationAngleDeg(90);
		roationlocklid4.SetPosition(b2Vec2(2895,180),0);
		if (closeT5) {
			PhysicsBody::m_bodiesToDelete.push_back(enddoor5);
			PhysicsBody::m_bodiesToDelete.push_back(enddoor6);
			closeT5 = false;
		}

	}
	if (!lock4.onlock8) {
		roationlocklid4.SetRotationAngleDeg(0);
		roationlocklid4.SetPosition(b2Vec2(2950, 250), 0);
	}

	//lock3
	if (lock3.onlock2) {
		roationlocklid3.SetRotationAngleDeg(0);
		roationlocklid3.SetPosition(b2Vec2(1930, 210), 0);
		roationlocklid3sptire.SetWidth(90);
	}
	else if(!lock3.onlock2){
		roationlocklid3.SetRotationAngleDeg(90);
		roationlocklid3.SetPosition(b2Vec2(1970, 230), 0);
		roationlocklid3sptire.SetWidth(70);

	}
	// lock 2

	if (lock2.onlock3) {
		roationlocklid2.SetRotationAngleDeg(90);
		roationlocklid2.SetPosition(b2Vec2(805, 298), 0);
		roationlocklid2sptire.SetWidth(90);
	}
	else if (!lock2.onlock3) {
		roationlocklid2.SetRotationAngleDeg(0);
		roationlocklid2.SetPosition(b2Vec2(795, 345), 0);
		roationlocklid2sptire.SetWidth(70);

	}


	// lock 1

	if (lock1.onlock4) {
		roationlocklid1.SetRotationAngleDeg(90);
		roationlocklid1.SetPosition(b2Vec2(1425,0), 0);
		roationlocklid1sptire.SetWidth(5);
		roationlocklid1sptire.SetHeight(50);
	}
	else if (!lock1.onlock4) {
		roationlocklid1.SetRotationAngleDeg(0);
		roationlocklid1.SetPosition(b2Vec2(1455,15), 0);
		roationlocklid1sptire.SetWidth(5);
		
	}

	

	if (position.x >= 205 && position.x <= 225 && counterHolder == 0)
	{

		
		Scene::CreatePhysiscsSprite(true, false, true, "boxSprite.jpg", 5, 90, 1, 148.f, 45.f, 0, 0, 0.3, 0.3);
		Scene::CreatePhysiscsSprite(true, false, true, "boxSprite.jpg", 5, 90, 1, 138.f, 45.f, 0, 0, 0.3, 0.3);

		
		counterHolder++;


	}

	if (position.x >= 715 && position.x <= 725 && counterHolder == 1)
	{
		Scene::CreatePhysiscsSprite(true, false, true, "door.png", 60, 10, 2, 694, 25, 90, 1, 0.3, 0.3);

		
		counterHolder++;


	}

	if (position.x >= 2010 && position.x <= 2015 && counterHolder == 2)
	{
		Scene::CreatePhysiscsSprite(true, false, true, "door.png", 60, 10, 2, 2000, 25, 90, 1, 0.3, 0.3);


		counterHolder++;


	}

	if (position.x >= 2455 && position.x <= 2545 && position.y >= 290 && position.y <= 320 && counterHolder == 0) {

		winconditon.SetTransparency(1);
		counterHolder++;
	}

}


void PhysicsPlayground::KeyboardHold()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	auto& sprite = ECS::GetComponent<Sprite>(MainEntities::MainPlayer());
	float speed = 1.f;
	
	int spriteHeight;
	int spriteWidth;

	b2Vec2 vel = b2Vec2(0.f, 0.f);

	if (Input::GetKey(Key::Shift))
	{
		speed *= 5.f;
	}

	if (Input::GetKey(Key::A))
	{
		player.GetBody()->ApplyForceToCenter(b2Vec2(-100000.f * speed, 0.f), true);
	}
	if (Input::GetKey(Key::D))
	{
		player.GetBody()->ApplyForceToCenter(b2Vec2(100000.f * speed, 0.f), true);
	}

	//Change physics body size for circle
	if (Input::GetKey(Key::O))
	{
		player.ScaleBody(1.3 * Timer::deltaTime, 0);
		spriteWidth = player.GetWidth(); // scaling up and down
		spriteHeight = player.GetHeight();
		sprite.SetWidth(spriteWidth);
		sprite.SetHeight(spriteHeight);

	}
	else if (Input::GetKey(Key::I))
	{
		player.ScaleBody(-1.3 * Timer::deltaTime, 0);
		spriteWidth = player.GetWidth();
		spriteHeight = player.GetHeight();
		sprite.SetWidth(spriteWidth);
		sprite.SetHeight(spriteHeight);
	}
}

void PhysicsPlayground::KeyboardDown()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	auto& canJump = ECS::GetComponent<CanJump>(MainEntities::MainPlayer());
	auto& Translate = ECS::GetComponent<TranslateT>(MainEntities::MainPlayer());
	auto& door2 = ECS::GetComponent<OnLock>(door2CloseT);
	auto& destroywalls1 = ECS::GetComponent<OnLock>(destroywall1);
	auto& destroywalls2 = ECS::GetComponent<OnLock>(destorywall2);
	auto& leverdown = ECS::GetComponent<Sprite>(lever1);
	auto& leverup = ECS::GetComponent<Sprite>(lever2);
	auto& leverleft = ECS::GetComponent<Sprite>(lever3);
	auto& leverright = ECS::GetComponent<Sprite>(lever4);
	auto& leverupsiderleft = ECS::GetComponent<Sprite>(lever5);
	auto& leverupsideright = ECS::GetComponent<Sprite>(lever6);
	
	


		

	

	//Teleportation look at TranslateTrigger.cpp
	if (Input::GetKeyDown(Key::F)) {

		if (destroywalls1.onlock6) {
			leverupsiderleft.SetTransparency(0);
			leverupsideright.SetTransparency(1);
			if (closeT3) {
				PhysicsBody::m_bodiesToDelete.push_back(enddoor1);
				PhysicsBody::m_bodiesToDelete.push_back(enddoor2);
				closeT3 = false;
			}
		}

		if(destroywalls2.onlock7) {
			leverleft.SetTransparency(0);
			leverright.SetTransparency(1);
			if (closeT4) {
				PhysicsBody::m_bodiesToDelete.push_back(enddoor3);
				PhysicsBody::m_bodiesToDelete.push_back(enddoor4);
				closeT4 = false;
			}
		}

		if (door2.onlock5) {
			leverup.SetTransparency(1);
			leverdown.SetTransparency(0);
			if (closeT2) {
				PhysicsBody::m_bodiesToDelete.push_back(door2e);
				closeT2 = false;
			}
		}
		
		if (Translate.teleport1) {
			player.SetPosition(b2Vec2(440, 165), 0);
		}

		else if (Translate.teleport2) {
			player.SetPosition(b2Vec2(520, 5), 0);
		}
	}
	//Gravity change and rotate
	if (Input::GetKeyDown(Key::R)) {
		if (m_gravity == b2Vec2(0.f, -98.f)) {
			m_gravity = b2Vec2(0.f, 98.f);
			m_physicsWorld->SetGravity(m_gravity);
			player.SetRotationAngleDeg(180, 0);
		}
		else if (m_gravity == b2Vec2(0.f, 98.f)) {
			m_gravity = b2Vec2(0.f, -98.f);
			m_physicsWorld->SetGravity(m_gravity);
			player.SetRotationAngleDeg(0, 0);
		}
	}

	if (Input::GetKeyDown(Key::T))
	{
		PhysicsBody::SetDraw(!PhysicsBody::GetDraw());
	}


	if (canJump.m_canJump)
	{
		if (Input::GetKeyDown(Key::Space))
		{
			if (m_gravity == b2Vec2(0.f, -98.f)) {
				player.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(0.f, 160000.f), true);
				canJump.m_canJump = false;
			}
			//Jumping when gravity 
			else if (m_gravity == b2Vec2(0.f, 98.f)) {
				player.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(0.f, -160000.f), true);
				canJump.m_canJump = false;
			}
		}
	}
}

void PhysicsPlayground::KeyboardUp()
{
	

}
