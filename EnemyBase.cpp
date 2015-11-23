#include "Enemybase.h"

// Enemy Implementation
namespace Enemy_Space {

	static int enemy_num;

	Enemy::Enemy(void) : enemyHealth(10), boundingSphereRadius(4) {
	}

	Enemy::~Enemy (void) {
		destoryEnemy();
	}

	// Enemy Constructor
	Enemy::Enemy (Ogre::SceneManager* scene_man, Ogre::String name, Ogre::Vector3 initalPosition, int enemy_rep) : scene_manager(scene_man), alive(true), enemyHealth(10), original_position(initalPosition){
		shot = nullptr;
		currentDirection = Ogre::Vector3(0.0,0.0,0.0);
		createEnemyByNum(scene_man, name, initalPosition, enemy_rep);
		buildPointGraph();
		STATE = PROWL;
		wait = 0.0;
		lifeSpan = 0.0;
	}

	// Builds The Graph That This Enemy Will Use To Move
	void Enemy::buildPointGraph(void) {
		currentPathIndex = 0;
		pathPoints.push_back(Ogre::Vector3 (original_position.x + 10, original_position.y, original_position.z - 15));
		pathPoints.push_back(Ogre::Vector3 (original_position.x - 10, original_position.y, original_position.z - 15));
	}

	// Moves The Enemey One Frame In It's Cycle
	void Enemy::advance(Player_Space::Player* player) {
		if (ship_node) {
			if(lifeSpan > 1000){
				maintainFiringRange(player->getPosition());
			}

			//maintainFiringRange(player->getPosition());
			// Finally Move Our Lazer If It Exists
			moveLazer();

			if(STATE == IDLE){
				currentDirection = pathPoints[currentPathIndex  % pathPoints.size()] - ship_node->getPosition();
				currentDirection.normalise();
				ship_node->translate(currentDirection * ENEMY_MOVE_SPEED);

				// Call The Graph Cycler 
				cycleGraphPointIfNeeded();

			}
			else if(STATE == PROWL){
				prowl();
				spotPlayer(player->getPosition());
			}

			else if(STATE == PURSUE){				
				pursue(player->getPosition());
				maintainFiringRange(player->getPosition());
			}
			else if(STATE == FLEE){
				flee(player->getPosition());
				maintainFiringRange(player->getPosition());
			}
			else if(STATE == HALT){

			}

			lifeSpan += 1;
			wait -= 1;
		}
	}

	void Enemy::prowl(){
		if( wait <= 0 ){
			currentDirection = RandomVector3();
			currentDirection.normalise();
			wait = 100;
		}
		ship_node->translate(currentDirection * ENEMY_MOVE_SPEED);

	}

	void Enemy::spotPlayer(Ogre::Vector3 playerPos){
		float distance = GetMagnatude(GetVectorFromTwoPoints(playerPos, ship_node->getPosition()));
		if(distance <= 100.0){
			STATE = PURSUE;
		}
	}

	void Enemy::pursue(Ogre::Vector3 playerPos){
		Ogre::Vector3 newDirection = GetVectorFromTwoPoints(playerPos, ship_node->getPosition());
		newDirection.normalise();
		Ogre::Vector3 axis = currentDirection.crossProduct(newDirection);
		axis.normalise();
		Ogre::Radian angle = Ogre::Radian(currentDirection.dotProduct(newDirection));

		currentDirection = newDirection;
		//RotateShip(axis, angle);
		ship_node->translate(currentDirection * ENEMY_MOVE_SPEED);
		
		
	}
	void Enemy::flee(Ogre::Vector3 playerPos){
		Ogre::Vector3 newDirection = GetVectorFromTwoPoints(playerPos, ship_node->getPosition());
		newDirection.normalise();
		Ogre::Vector3 axis = currentDirection.crossProduct(newDirection);
		axis.normalise();
		Ogre::Radian angle = Ogre::Radian(currentDirection.dotProduct(newDirection));

		currentDirection = newDirection;
		//RotateShip(axis, angle);
		ship_node->translate(currentDirection * ENEMY_MOVE_SPEED * (-1));
		
	}

	void Enemy::maintainFiringRange(Ogre::Vector3 playerPos){
		float distance = GetMagnatude(GetVectorFromTwoPoints(playerPos, ship_node->getPosition()));
		if(distance <= 15.0 && distance > 10.0 ){
			STATE = HALT;
		}
		else if( distance <= 15.0){
			STATE = FLEE;
		}
		else if( distance > 10.0 ){
			STATE = PURSUE;
		}
		
	}
	

	// Cycles To The Next  Graph Point
	void Enemy::cycleGraphPointIfNeeded(void) {
		if (ship_node->getPosition().squaredDistance(pathPoints[currentPathIndex % pathPoints.size()]) < NODE_MIN_RADIUS) {
			currentPathIndex++;
		}
	}

	// Destories The Enemy And Removes Them From The Scene 
	void Enemy::destoryEnemy(void) {
		if (alive) {
			Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();
			root_scene_node->removeAndDestroyChild(ship_node->getName());
			scene_manager->destroyEntity(entity_name);
			ship_node = 0;
			alive = false;

			if (shot) {
				shot->destoryFiredWeapon();
			}
		}
	}

	// Registers A Hit On The Enemy 
	void Enemy::registerHit (int damageAmount) {
		enemyHealth = std::max (enemyHealth - damageAmount, 0);
		std::cout << "Hit Enemy... Current Health Is: " << enemyHealth << std::endl;
	}

	// Fires A Shot If Player Is Within The Bubble
	void Enemy::shouldFireShot(Player_Space::Player* enemyPlayer) {
		float distanceBetween = getPosition().distance(enemyPlayer->getPosition());
		if (distanceBetween < PLAYER_HOSTILE_RADIUS) {
			fireShot();
		}
	}

	// Fires A Shot When Called
	void Enemy::fireShot(void) {
		if (!shot) {
			shot = new Weapon_Space::Weapon_Shot (scene_manager, ship_node->getPosition(), currentDirection, entity_name);
		}
	}

	boolean Enemy::enemyDead(void) {
		return enemyHealth == 0;
	}

	int Enemy::getEnemyHealth(void) {
		return enemyHealth;
	}

	float Enemy::getBoundingCircleRadius(void) {
		return boundingSphereRadius;
	}

	/*-------------------------------------------- Enemy Node Functions ----------------------------------*/
	void Enemy::createEnemyByNum (Ogre::SceneManager* scene_manager, Ogre::String name, Ogre::Vector3 pos, int num) {
		switch (num) {
		case 0: CreateEnemy1(scene_manager, name, pos); break;
		case 1: CreateEnemy2(scene_manager, name, pos); break;
		case 2: CreateEnemy3(scene_manager, name, pos); break;
		case 3: CreateEnemy4(scene_manager, name, pos); break;
		case 4: CreateEnemy5(scene_manager, name, pos); break;
		}
	}

	// Creates The Enemy When Called
	void Enemy::CreateEnemy1 (Ogre::SceneManager* manager, Ogre::String name, Ogre::Vector3 initalPosition) {
		Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();
		Ogre::Entity *entity = scene_manager->createEntity("test" + enemy_num++, "cube.mesh");

		entity_name = "enemy" + enemy_num;
		ship_node = root_scene_node->createChildSceneNode(entity_name);
		ship_node->attachObject(entity);

		boundingSphereRadius = 4;
		ship_node->setPosition(initalPosition);
		ship_node->setScale (boundingSphereRadius, boundingSphereRadius, boundingSphereRadius);
	}

	// Create Our Second Entity Mesh
	void Enemy::CreateEnemy2(Ogre::SceneManager* scene_manager, Ogre::String name, Ogre::Vector3 pos){
		Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();
		Ogre::Entity *entity;

		Ogre::String entity_name = "enemy1" + enemy_num;
		entity = scene_manager->createEntity(entity_name, "Enemy1");
		ship_node = root_scene_node->createChildSceneNode("enemy1");
		ship_node->attachObject(entity);

		boundingSphereRadius = 25;
		ship_node->setPosition(pos);
		ship_node->setScale (boundingSphereRadius, boundingSphereRadius, boundingSphereRadius);
	}

	// Create Our Third Type Of Enemy
	void Enemy::CreateEnemy3(Ogre::SceneManager* scene_manager, Ogre::String name, Ogre::Vector3 pos){
		Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();
		Ogre::Entity *entity;
		Ogre::Matrix4 transformations;
		Ogre::String entity_name;

		entity_name = "Enemy2_body";
		entity = scene_manager->createEntity(entity_name, "cube.mesh");
		ship_node = root_scene_node->createChildSceneNode("Enemy2_body");
		ship_node->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(2.0, 2.0, 2.0));
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(0.0, 0.0, 0.0))) * transformations;
		AssignTransf(ship_node, transformations);

		entity_name = "rightarm2";
		entity = scene_manager->createEntity(entity_name, "cube.mesh");  //mesh name on the right, entity on the left
		Ogre::SceneNode* rightArm2 = ship_node->createChildSceneNode("rightarm2");
		rightArm2->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(0.5, 0.5, 0.5));
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(-1.25, 0.0, 0.0))) * transformations;
		AssignTransf(rightArm2, transformations);

		entity_name = "leftarm2";
		entity = scene_manager->createEntity(entity_name, "cube.mesh");  //mesh name on the right, entity on the left
		Ogre::SceneNode* leftArm2 = ship_node->createChildSceneNode("leftarm2");
		leftArm2->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(0.5, 0.5, 0.5));
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(1.25, 0.0, 0.0))) * transformations;
		AssignTransf(leftArm2, transformations);

		entity_name = "rightcannon2";
		entity = scene_manager->createEntity(entity_name, "Cylinder");  //mesh name on the right, entity on the left
		Ogre::SceneNode* rightCannon = ship_node->createChildSceneNode("rightcannon2");
		rightCannon->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(0.25, 0.25, 0.25));
		transformations = Ogre::Matrix4(RotationMatrix(Ogre::Vector3(1.0, 0.0, 0.0), Ogre::Radian(Ogre::Math::PI/2.0))) * transformations;
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(-1.25, 0.0, -0.5))) * transformations;
		AssignTransf(rightCannon, transformations);

		entity_name = "leftcannon2";
		entity = scene_manager->createEntity(entity_name, "Cylinder");  //mesh name on the right, entity on the left
		Ogre::SceneNode* leftCannon = ship_node->createChildSceneNode("leftcannon2");
		leftCannon->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(0.25, 0.25, 0.25));
		transformations = Ogre::Matrix4(RotationMatrix(Ogre::Vector3(1.0, 0.0, 0.0), Ogre::Radian(Ogre::Math::PI/2.0))) * transformations;
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(1.25, 0.0, -0.5))) * transformations;
		AssignTransf(leftCannon, transformations);

		boundingSphereRadius = 10;
		ship_node->setPosition(pos);
	}

	void Enemy::CreateEnemy4(Ogre::SceneManager* scene_manager, Ogre::String name, Ogre::Vector3 pos){

		Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();
		Ogre::Entity *entity;
		Ogre::Matrix4 transformations;
		Ogre::String entity_name;

		entity_name = name + " " + "body3";
		entity = scene_manager->createEntity(entity_name, "Prism");  //mesh name on the right, entity on the left
		ship_node = root_scene_node->createChildSceneNode("body3");
		ship_node->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(2.0, 2.0, 2.0));
		transformations = Ogre::Matrix4(RotationMatrix(Ogre::Vector3(0.0, 0.0, -1.0), Ogre::Radian(Ogre::Math::PI))) * transformations;
		AssignTransf(ship_node, transformations);

		entity_name = name + " " + "toparm3";
		entity = scene_manager->createEntity(entity_name, "Cylinder");  //mesh name on the right, entity on the left
		Ogre::SceneNode* topArm = ship_node->createChildSceneNode("toparm3");
		topArm->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(0.6, 0.3, 0.6));
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(1.0, 1.5, 1.75))) * transformations;
		AssignTransf(topArm, transformations);

		entity_name = name + " " + "bottomleftarm3";
		entity = scene_manager->createEntity(entity_name, "Cylinder");  //mesh name on the right, entity on the left
		Ogre::SceneNode* bottomLeftArm = ship_node->createChildSceneNode("bottomleftarm3");
		bottomLeftArm->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(0.6, 0.6, 0.6));
		transformations = Ogre::Matrix4(RotationMatrix(Ogre::Vector3(0.0, 0.0, -1.0), Ogre::Radian(Ogre::Math::PI/2))) * transformations;
		transformations = Ogre::Matrix4(RotationMatrix(Ogre::Vector3(0.0, 0.0, -1.0), Ogre::Radian(Ogre::Math::PI/4))) * transformations;
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(2.5, -1.0, 1.75))) * transformations;
		AssignTransf(bottomLeftArm, transformations);

		entity_name = name + " " + "bottomrightarm3";
		entity = scene_manager->createEntity(entity_name, "Cylinder");  //mesh name on the right, entity on the left
		Ogre::SceneNode* bottomRightArm = ship_node->createChildSceneNode("bottomrightarm3");
		bottomRightArm->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(0.6, 0.6, 0.6));
		transformations = Ogre::Matrix4(RotationMatrix(Ogre::Vector3(0.0, 0.0, 1.0), Ogre::Radian(Ogre::Math::PI/2))) * transformations;
		transformations = Ogre::Matrix4(RotationMatrix(Ogre::Vector3(0.0, 0.0, 1.0), Ogre::Radian(Ogre::Math::PI/4))) * transformations;
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(0.0, -1.0, 1.75))) * transformations;
		AssignTransf(bottomRightArm, transformations);

		entity_name = name + " " + "topgun3";
		entity = scene_manager->createEntity(entity_name, "Prism");  //mesh name on the right, entity on the left
		Ogre::SceneNode* topGun = ship_node->createChildSceneNode("topgun3");
		topGun->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(0.8, 0.8, 4.0));		
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(1.30, 2.5, -1.0))) * transformations;
		AssignTransf(topGun, transformations);

		entity_name = name + " " + "bottomleftgun3";
		entity = scene_manager->createEntity(entity_name, "Prism");  //mesh name on the right, entity on the left
		Ogre::SceneNode* bottomLeftGun = ship_node->createChildSceneNode("bottomleftgun3");
		bottomLeftGun->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(0.8, 0.8, 4.0));		
		transformations = Ogre::Matrix4(RotationMatrix(Ogre::Vector3(0.0, 0.0, 1.0), Ogre::Radian(Ogre::Math::PI/2))) * transformations;
		transformations = Ogre::Matrix4(RotationMatrix(Ogre::Vector3(0.0, 0.0, 1.0), Ogre::Radian(Ogre::Math::PI/4))) * transformations;
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(3.0, -1.5, -1.0))) * transformations;
		AssignTransf(bottomLeftGun, transformations);

		entity_name = name + " " + "bottomrightgun3";
		entity = scene_manager->createEntity(entity_name, "Prism");  //mesh name on the right, entity on the left
		Ogre::SceneNode* bottomRightGun = ship_node->createChildSceneNode("bottomrightgun3");
		bottomRightGun->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(0.8, 0.8, 4.0));		
		transformations = Ogre::Matrix4(RotationMatrix(Ogre::Vector3(0.0, 0.0, -1.0), Ogre::Radian(Ogre::Math::PI/2))) * transformations;
		transformations = Ogre::Matrix4(RotationMatrix(Ogre::Vector3(0.0, 0.0, -1.0), Ogre::Radian(Ogre::Math::PI/4))) * transformations;
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(-1.25, -2.25, -1.0))) * transformations;
		AssignTransf(bottomRightGun, transformations);

		boundingSphereRadius = 10;
		ship_node->setPosition(pos);
		ship_node->setScale (boundingSphereRadius, boundingSphereRadius, boundingSphereRadius);
	}

	void Enemy::CreateEnemy5(Ogre::SceneManager* scene_manager, Ogre::String name, Ogre::Vector3 pos){

		Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();
		Ogre::Entity *entity;
		Ogre::Matrix4 transformations;
		Ogre::String entity_name;


		entity_name = "bodyone4";
		entity = scene_manager->createEntity(entity_name, "cube.mesh");
		ship_node = root_scene_node->createChildSceneNode("bodyone4");
		ship_node->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(2.0, 0.5, 2.0));
		AssignTransf(ship_node, transformations);

		entity_name = "bodytwo4";
		entity = scene_manager->createEntity(entity_name, "cube.mesh");
		Ogre::SceneNode* shipbody2 = ship_node->createChildSceneNode("toparm3");
		shipbody2 = root_scene_node->createChildSceneNode("bodytwo4");
		shipbody2->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(2.0, 0.5, 2.0));
		transformations = Ogre::Matrix4(RotationMatrix(Ogre::Vector3(0.0, 0.0, 1.0), Ogre::Radian(Ogre::Math::PI/2.0))) * transformations;

		AssignTransf(shipbody2, transformations);

		entity_name = "rightwing4";
		entity = scene_manager->createEntity(entity_name, "Prism");
		Ogre::SceneNode* rightwing = ship_node->createChildSceneNode("rightwing4");
		rightwing->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(0.5, 1.0, 2.4));
		transformations = Ogre::Matrix4(RotationMatrix(Ogre::Vector3(0.0, 0.0, 1.0), Ogre::Radian(Ogre::Math::PI/2.0))) * transformations;
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(-1.5, 0.25, -1.0))) * transformations;
		AssignTransf(rightwing, transformations);

		entity_name = "leftwing4";
		entity = scene_manager->createEntity(entity_name, "Prism");
		Ogre::SceneNode* leftwing = ship_node->createChildSceneNode("leftwing");
		leftwing->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(0.5, 1.0, 2.4));
		transformations = Ogre::Matrix4(RotationMatrix(Ogre::Vector3(0.0, 0.0, -1.0), Ogre::Radian(Ogre::Math::PI/2.0))) * transformations;
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(1.5, -0.25, -1.0))) * transformations;
		AssignTransf(leftwing, transformations);

		entity_name = "topwing4";
		entity = scene_manager->createEntity(entity_name, "Prism");
		Ogre::SceneNode* topwing = ship_node->createChildSceneNode("topwing4");
		topwing->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(0.5, 1.0, 2.4));
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(0.25, 1.5, -1.0))) * transformations;
		AssignTransf(topwing, transformations);

		entity_name = "bottomwing4";
		entity = scene_manager->createEntity(entity_name, "Prism");
		Ogre::SceneNode* bottomwing = ship_node->createChildSceneNode("bottomwing4");
		bottomwing->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(0.5, 1.0, 2.4));
		transformations = Ogre::Matrix4(RotationMatrix(Ogre::Vector3(0.0, 0.0, -1.0), Ogre::Radian(Ogre::Math::PI))) * transformations;
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(-0.25, -1.5, -1.0))) * transformations;
		AssignTransf(bottomwing, transformations);

		entity_name = "topgun4";
		entity = scene_manager->createEntity(entity_name, "Cylinder");
		Ogre::SceneNode* topgun = ship_node->createChildSceneNode("topgun4");
		topgun->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(0.5, 0.25, 0.5));
		transformations = Ogre::Matrix4(RotationMatrix(Ogre::Vector3(1.0, 0.0, 0.0), Ogre::Radian(Ogre::Math::PI/2))) * transformations;
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(0.0, 0.65, -1.0))) * transformations;
		AssignTransf(topgun, transformations);

		entity_name = "bottomgun4";
		entity = scene_manager->createEntity(entity_name, "Cylinder");
		Ogre::SceneNode* bottomgun = ship_node->createChildSceneNode("bottomgun4");
		bottomgun->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(0.5, 0.25, 0.5));
		transformations = Ogre::Matrix4(RotationMatrix(Ogre::Vector3(1.0, 0.0, 0.0), Ogre::Radian(Ogre::Math::PI/2))) * transformations;
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(0.0, -0.65, -1.0))) * transformations;
		AssignTransf(bottomgun, transformations);

		entity_name = "leftgun4";
		entity = scene_manager->createEntity(entity_name, "Cylinder");
		Ogre::SceneNode* leftgun = ship_node->createChildSceneNode("leftgun4");
		leftgun->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(0.5, 0.25, 0.5));
		transformations = Ogre::Matrix4(RotationMatrix(Ogre::Vector3(1.0, 0.0, 0.0), Ogre::Radian(Ogre::Math::PI/2))) * transformations;
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(0.65, 0.0, -1.0))) * transformations;
		AssignTransf(leftgun, transformations);

		entity_name = "rightgun4";
		entity = scene_manager->createEntity(entity_name, "Cylinder");
		Ogre::SceneNode* rightgun = ship_node->createChildSceneNode("rightgun4");
		rightgun->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(0.5, 0.25, 0.5));
		transformations = Ogre::Matrix4(RotationMatrix(Ogre::Vector3(1.0, 0.0, 0.0), Ogre::Radian(Ogre::Math::PI/2))) * transformations;
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(-0.65, 0.0, -1.0))) * transformations;
		AssignTransf(rightgun, transformations);

		boundingSphereRadius = 10;
		ship_node->setPosition(pos);
		ship_node->setScale (3, 3, 3);
	}




	/*-------------------------------------------- Helper Functions --------------------------------------*/

	// Rotate enemy ship using an orbit transformation
	void Enemy::RotateShip(Ogre::Vector3 axis, Ogre::Radian  degree){
		Ogre::Vector3 playerPos = ship_node->getPosition();
		Ogre::Vector3 zeroVector = Ogre::Vector3(0.0, 0.0, 0.0);
		Ogre::Vector3 displacement = zeroVector - playerPos;
		Ogre::Matrix4 transformations;

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = Ogre::Matrix4(TranslationMatrix(displacement)) * transformations;
		UpdateTransf(ship_node, transformations);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = Ogre::Matrix4(RotationMatrix(axis, Ogre::Radian(degree))) * transformations;
		UpdateTransf(ship_node, transformations);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = Ogre::Matrix4(TranslationMatrix(playerPos)) * transformations;
		UpdateTransf(ship_node, transformations);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = Ogre::Matrix4(RotationMatrix(axis, Ogre::Radian(degree))) * transformations;
		UpdateTransf(ship_node, transformations);

	}

	Ogre::Vector3 Enemy::RandomVector3(void){
		Ogre::Vector3 direction;
		direction.x = rand() % 200 + (-100);
		direction.y = rand() % 200 + (-100);
		direction.z = rand() % 200 + (-100);
		return direction;
	}

	float Enemy::GetMagnatude(Ogre::Vector3 v){
		float r = sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
		return r;
	}

	// Create a vector from two points
	Ogre::Vector3 Enemy::GetVectorFromTwoPoints(Ogre::Vector3 playerpos, Ogre::Vector3 Enemypos){
		Ogre::Vector3  playerdirection = (playerpos - Enemypos);
		return playerdirection;
	}

	// Create a rotation matrix based on an angle and an axis
	Ogre::Matrix4 Enemy::RotationMatrix(Ogre::Vector3 axis, Ogre::Radian angle){

		Ogre::Matrix3 mat;
		mat = Ogre::Matrix3::IDENTITY;
		mat.FromAngleAxis(axis, angle);
		return Ogre::Matrix4(mat);
	}


	// Create a translation matrix based on a vector of translations (x, y, z)
	Ogre::Matrix4 Enemy::TranslationMatrix(Ogre::Vector3 trans){
		Ogre::Matrix4 mat;
		mat = Ogre::Matrix4::IDENTITY;
		mat.setTrans(trans);
		return mat;
	}


	// Create a scaling matrix based on a vector of scale factors (x, y, z)
	Ogre::Matrix4 Enemy::ScalingMatrix(Ogre::Vector3 scale){
		Ogre::Matrix4 mat;
		mat = Ogre::Matrix4::IDENTITY;
		mat.setScale(scale);
		return mat;
	}

	void Enemy::AssignTransf(Ogre::SceneNode* node, Ogre::Matrix4 transf){

		/* In many graphic frameworks, we would simply multiply our geometry by the transformation matrix.
		However, OGRE stores the transformations of a node in a more efficient manner.
		So, we need to decompose the transformation first into three components and then assign them
		to the scene node.*/
		Ogre::Vector3 trans, scale;
		Ogre::Quaternion quat;

		transf.decomposition(trans, scale, quat);
		node->setScale(scale);
		node->setOrientation(quat);
		node->setPosition(trans);
	}

	void Enemy::UpdateTransf(Ogre::SceneNode* node, Ogre::Matrix4 transf){

		/* In many graphic frameworks, we would simply multiply our geometry by the transformation matrix.
		However, OGRE stores the transformations of a node in a more efficient manner.
		So, we need to decompose the transformation first into three components and then assign them
		to the scene node.*/
		Ogre::Vector3 trans, scale;
		Ogre::Quaternion quat;

		transf.decomposition(trans, scale, quat);
		node->scale(scale);
		node->rotate(quat);
		node->translate(trans);
	}
}