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
		tempDef.position.Set(float32(-35.f), float32(15.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		//tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);
		tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight() - shrinkY)/2.f), vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);

		tempPhsBody.SetRotationAngleDeg(0.f);
		tempPhsBody.SetFixedRotation(true);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
		tempPhsBody.SetGravityScale(1.f);

		
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
	ECS::GetComponent<Trigger*>(entity) = new DestroyTrigger();
	ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
	ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(box1);

	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

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
	
	//Floor1
	Scene::CreatePhysiscsSprite(true, false, true, "boxSprite.jpg", 340, 15, 0, 30.f, -10.f, 0, 1, 0.3, 0.3);

	//Wall
	Scene::CreatePhysiscsSprite(true, true, false, "boxSprite.jpg", 300, 1000, 100, -300.f, 250.f, 90, 1, 0.3, 0.3);

	//Door1
	Scene::CreatePhysiscsSprite(false, false, true, "boxSprite.jpg", 80, 10, 0, 155.f, 100.f, 90, 1, 0.3, 0.3);

	//Floor Down 2
	Scene::CreatePhysiscsSprite(true, false, true, "boxSprite.jpg", 900, 15, 0, 250.f, -10.f, 0, 1, 0.3, 0.3);
	
	//First Ramp
	Scene::CreatePhysiscsSprite(true, false, true, "boxSprite.jpg", 40, 48, 0, 370.f, 12.f, 0, 1, 0.3, 0.3); 

	//Teleporter Down
	Scene::CreatePhysiscsSprite(true, false, true, "boxSprite.jpg", 50, 5, 0, 520.f, 0.f, 0, 1, 0.3, 0.3);

	//Crate 1
	Scene::CreatePhysiscsSprite(false, false, true, "boxSprite.jpg", 25, 25, 0, 580.f, 12.f, 0, 1, 0.3, 0.3);

	//Floor Up 1
	Scene::CreatePhysiscsSprite(true, false, true, "boxSprite.jpg", 100, 15, 0, 410.f, 150.f, 0, 1, 0.3, 0.3);
	
	//Teleporter Up
	Scene::CreatePhysiscsSprite(true, false, true, "boxSprite.jpg", 40, 5, 0, 440.f, 160.f, 0, 1, 0.3, 0.3);

	//Second Ramp
	Scene::CreatePhysiscsSprite(true, false, true, "boxSprite.jpg", 100, 15, 0, 330.f, 175.f, -30, 1, 0.3, 0.3);

	//Floor Up 2
	Scene::CreatePhysiscsSprite(true, false, true, "boxSprite.jpg", 100, 15, 0, 250.f, 200.f, 0, 1, 0.3, 0.3);

	//Button 2
	Scene::CreatePhysiscsSprite(true, false, true, "boxSprite.jpg", 50, 5, 0, 200.f, 230.f, 90, 1, 0.3, 0.3);

	//Second door
	Scene::CreatePhysiscsSprite(true, false, true, "boxSprite.jpg", 80, 10, 0, 694.f, 100.f, 90, 1, 0.3, 0.3); // make duplicate with y = 25.f later for closed door or dynamic body trick, either make a duplicate and make it a trigger to destroy it, then trigger another to show up after they pass the door
	
	//Wall 2
	Scene::CreatePhysiscsSprite(true, false, true, "boxSprite.jpg", 400, 50, 1, 694.f, 250.f, 90, 1, 0.3, 0.3);

	//Floor 3
	Scene::CreatePhysiscsSprite(true, false, true, "boxSprite.jpg", 900, 15, 0, 950.f, -10.f, 0, 1, 0.3, 0.3);

	//Floor 4
	Scene::CreatePhysiscsSprite(true, false, true, "boxSprite.jpg", 600, 15, 0, 1750.f, -10.f, 0, 1, 0.3, 0.3);

	//Lock1 
	Scene::CreatePhysiscsSprite(true, false, true, "LinkStandby", 55, 48, 0, 1425.f, -40.f, 0, 1, 0.3, 0.3);

	//Wall 3
	Scene::CreatePhysiscsSprite(true, false, true, "boxSprite.jpg", 400, 50, 1, 2000.f, 250.f, 90, 1, 0.3, 0.3);

	//Third door
	Scene::CreatePhysiscsSprite(true, false, true, "boxSprite.jpg", 80, 10, 0, 2000.f, 80.f, 90, 1, 0.3, 0.3);

	//Floor 5
	Scene::CreatePhysiscsSprite(true, false, true, "boxSprite.jpg", 1000, 15, 0, 2500.f, -10.f, 0, 1, 0.3, 0.3);


	//Wall and tile for lock and barrier for box
	Scene::CreatePhysiscsSprite(true, false, true, "boxSprite.jpg", 100, 25, 1, 2888.f, 65.f, 90, 1, 0.3, 0.3); //wall right
	Scene::CreatePhysiscsSprite(true, false, true, "boxSprite.jpg", 150, 25, 1, 2950.f, 120.f, 0, 1, 0.3, 0.3); //ceilinga

	//Barrier for box and button
	Scene::CreatePhysiscsSprite(true, false, true, "boxSprite.jpg", 90, 10, 1, 2935.f, 40.f, 90, 1, 0.3, 0.3);

	Scene::CreatePhysiscsSprite(true, false, true, "boxSprite.jpg", 50, 5, 0, 2500.f, 285.f, 0, 1, 0.3, 0.3);
	
	//Wall 4
	Scene::CreatePhysiscsSprite(true, false, true, "boxSprite.jpg", 500, 50, 1, 3000.f, 225.f, 90, 1, 0.3, 0.3);

	//End Floor
	Scene::CreatePhysiscsSprite(true, false, true, "boxSprite.jpg", 100, 25, 1, 2500.f, 300.f, 0, 1, 0.3, 0.3);

	//Button 4
	Scene::CreatePhysiscsSprite(true, false, true, "boxSprite.jpg", 50, 5, 0, 2500.f, 285.f, 0, 1, 0.3, 0.3);

	//Roof
	Scene::CreatePhysiscsSprite(true, false, true, "boxSprite.jpg", 3000, 50, 1, 1500.f, 425.f, 0, 1, 0.3, 0.3);
	

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
		std::string fileName = "BeachBall.png";
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

	
	


	if (position.x >= 205 && position.x <= 225 && counterHolder == 0)
	{

		
		Scene::CreatePhysiscsSprite(true, false, true, "boxSprite.jpg", 5, 90, 1, 150.f, 45.f, 0, 0, 0.3, 0.3);
		std::cout << "Sprite is made";
		std::cout << position.x;
		counterHolder++;


	}

	if (position.x >= 227 && position.x <= 229 && counterHolder == 1)
	{


		Scene::CreatePhysiscsSprite(true, false, true, "boxSprite.jpg", 5, 90, 1, 140.f, 45.f, 0, 0, 0.3, 0.3);
		std::cout << "Sprite is made";
		std::cout << position.x;
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


	if (Input::GetKeyDown(Key::F)) {
		if (Translate.teleport1) {
			player.SetPosition(b2Vec2(440, 165), 0);
		}

		else if (Translate.teleport2) {
			player.SetPosition(b2Vec2(520, 5), 0);
		}
	}

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
