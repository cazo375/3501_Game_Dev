#include "Enemybase.h"

// Enemy Implementation
namespace Enemy_Space {
	// one bag of insert_profaine_item_here
	static int enemy_num = 0;

	Enemy::Enemy(void) : boundingSphereRadius(4) {
		health = 10;
	}

	Enemy::~Enemy (void) {
		destoryEnemy();
	}

	// Enemy Constructor
	Enemy::Enemy (Ogre::SceneManager* scene_man, Ogre::Vector3 initalPosition, int enemy_rep) : scene_manager(scene_man), alive(true), original_position(initalPosition){
		shot = nullptr;
		currentDirection = Ogre::Vector3(0.0,0.0,-1.0);
		initialDir = currentDirection;
		entity_name = "enemey" + Ogre::StringConverter::toString(enemy_num++);
		createEnemyByNum(scene_man, initalPosition, enemy_rep);
		buildPointGraph();
		wait = 0.0;
		lifeSpan = 0.0;
		if(enemy_rep == 1){
			ENEMY_TYPE = PEON;
			STATE = PROWL;
			health = 10;
			ship_node->scale(1.5, 1.5, 1.5);
			boundingSphereRadius = 5;
			sleep = rand() % 1700 + 700;
			currentWeaponIndex = 0;
			weapons.push_back(new Weapon_Space::Enemy_Lazer_Cannon(entity_name));
		}
		else if(enemy_rep == 2){
			ENEMY_TYPE = SCOUT;
			STATE = HALT;
			boundingSphereRadius = 8;
			health = 20;
			currentWeaponIndex = 0;
			weapons.push_back(new Weapon_Space::Lazer(entity_name));
		}
		else if(enemy_rep == 3){
			ENEMY_TYPE = BOSS;
			ship_node->scale(5.0, 5.0, 5.0);
			boundingSphereRadius = 17;
			STATE = PURSUE;
			health = 50;
			currentWeaponIndex = 0;
			weapons.push_back(new Weapon_Space::Scatter_Shot(entity_name));
		}
	}

	// Builds The Graph That This Enemy Will Use To Move
	void Enemy::buildPointGraph(void) {
		currentPathIndex = 0;
		pathPoints.push_back(Ogre::Vector3 (original_position.x + 10, original_position.y, original_position.z - 15));
		pathPoints.push_back(Ogre::Vector3 (original_position.x - 10, original_position.y, original_position.z - 15));
	}

	// Moves The Enemey One Frame In It's Cycle
	void Enemy::advance(Player_Space::Player* player, Ogre::Real time) {
		if (ship_node) {

			// Enemy State Scripts
			if(!isInArea()){
				STATE = RETURN;
			} else {
				if(ENEMY_TYPE == PEON){
					if(GetMagnatude(GetVectorFromTwoPoints(player->getPosition(), ship_node->getPosition())) <= SAFE_DISTANCE){
							STATE = FLEE;
					}
					else if(inHostileRange(player->getPosition())){
						STATE = INTIMIDATE;	
					}
					if(lifeSpan > sleep){
						if(!inHostileRange(player->getPosition())){
							STATE = PURSUE;
						}
					}
					else if(!inHostileRange(player->getPosition())){
						STATE = PROWL;
						spotPlayer(player->getPosition());
					}
				}

				else if(ENEMY_TYPE == SCOUT){
					if(GetMagnatude(GetVectorFromTwoPoints(player->getPosition(), ship_node->getPosition())) <= SAFE_DISTANCE){
							STATE = FLEE;
					}
					else if(inHostileRange(player->getPosition())){
						STATE = HALT;	
					}
					else if(!inHostileRange(player->getPosition())){
						STATE = PURSUE;
					}

				}

				if(ENEMY_TYPE == BOSS){
					if( ((int)(lifeSpan) % 1000) == 0 ){
						Ogre::Vector3 newDirection = GetVectorFromTwoPoints(player->getPosition(), ship_node->getPosition());
						newDirection.normalise();
						currentDirection = newDirection;
						RotateShip(currentDirection);
						wait = 250;
						STATE = CHARGE;
					}
				}
			}


			// State Commands
			if(STATE == IDLE){
				currentDirection = pathPoints[currentPathIndex  % pathPoints.size()] - ship_node->getPosition();
				currentDirection.normalise();
				ship_node->translate(currentDirection * ENEMY_MOVE_SPEED);

				// Call The Graph Cycler 
				cycleGraphPointIfNeeded();

			}
			else if(STATE == RETURN){
				returnToBattle();
			}
			else if(STATE == CHARGE){
				charge(player->getPosition());
			}
			else if(STATE == INTIMIDATE){
				intimidate(player->getPosition());
			}
			else if(STATE == PROWL){
				prowl(player->getPosition());
			}
			else if(STATE == PURSUE){				
				pursue(player->getPosition());
			}
			else if(STATE == FLEE){
				flee(player->getPosition());
			}
			else if(STATE == HALT){

			}

			// update clocks
			weapons[currentWeaponIndex]->advance(time);
			lifeSpan += 1;
			wait -= 1;
		}
	}

	void Enemy::returnToBattle(void){
		if(wait <= 0){
			currentDirection = toOrigin();
			RotateShip(currentDirection);
			wait = 150;
		}
		ship_node->translate(currentDirection * ENEMY_MOVE_SUPER_SPEED);
	}

	// Enemy charges at the player
	void Enemy::charge(Ogre::Vector3 playerPos){
		if(wait<= 0){
			STATE = PURSUE;
		}
		ship_node->translate(currentDirection * ENEMY_MOVE_SUPER_SPEED);
	}


	// Enemy flys by the player
	void Enemy::intimidate(Ogre::Vector3 playerPos){
		if( wait <= 0 ){
			Ogre::Vector3 playerDirection = GetVectorFromTwoPoints(playerPos, ship_node->getPosition());
			playerDirection.normalise();
			Ogre::Vector3 newDirection = GetVectorFromTwoPoints(playerPos, playerDirection.crossProduct(RandomVector3()));
			newDirection.normalise();
			currentDirection = newDirection;
			wait = 30;
			RotateShip(playerDirection);
		}
		ship_node->translate(currentDirection * ENEMY_MOVE_SUPER_SPEED);
	}

	// Have the enemy prowl in a random direction
	void Enemy::prowl(Ogre::Vector3 playerPos){
		if( wait <= 0 ){
			currentDirection = RandomVector3();
			currentDirection.normalise();
			RotateShip(currentDirection);
			wait = 200;
		}
		ship_node->translate(currentDirection * ENEMY_MOVE_SPEED);
	}

	// Checks To See If The Player Is Within Pursue Distance
	void Enemy::spotPlayer(Ogre::Vector3 playerPos){
		if(inSpotRange(playerPos)){
			STATE = PURSUE;
		}
	}

	// Called When Pursing Player
	void Enemy::pursue(Ogre::Vector3 playerPos){
		Ogre::Vector3 newDirection = GetVectorFromTwoPoints(playerPos, ship_node->getPosition());
		newDirection.normalise();
		currentDirection = newDirection;
		RotateShip(playerPos);
		if(ENEMY_TYPE == BOSS){
			ship_node->translate(currentDirection * ENEMY_MOVE_SLOW_SPEED);
		}else if(ENEMY_TYPE == SCOUT){
			ship_node->translate(currentDirection * ENEMY_MOVE_FAST_SPEED);
		}
		else {
			ship_node->translate(currentDirection * ENEMY_MOVE_SPEED);
		}
	}

	// Called When Attempting To Run From A Player
	void Enemy::flee(Ogre::Vector3 playerPos){
		Ogre::Vector3 newDirection = GetVectorFromTwoPoints(playerPos, ship_node->getPosition());
		newDirection.normalise();
		currentDirection = newDirection;
		RotateShip(currentDirection);
		ship_node->translate(currentDirection * ENEMY_MOVE_SPEED * (-1));

	}



	// Cycles To The Next Graph Point
	void Enemy::cycleGraphPointIfNeeded(void) {
		if (ship_node->getPosition().squaredDistance(pathPoints[currentPathIndex % pathPoints.size()]) < NODE_MIN_RADIUS) {
			currentPathIndex++;
		}
	}

	// Destories The Enemy And Removes Them From The Scene 
	void Enemy::destoryEnemy(void) {
		if (alive) {
			Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

			// Destory Our Entities
			Ogre::SceneNode::ObjectIterator it = ship_node->getAttachedObjectIterator();
			while (it.hasMoreElements())
			{
				Ogre::Entity* entity = static_cast<Ogre::Entity*>(it.getNext());
				scene_manager->destroyEntity(entity->getName());
			}

			// Finish Deleting Our Node
			root_scene_node->removeAndDestroyChild(ship_node->getName());

			ship_node = 0;
			alive = false;
		}

		for (int i = 0; i < weapons.size(); i++) {
			delete weapons[i];
		}
	}

	// Registers A Hit On The Enemy 
	void Enemy::registerHit (int damageAmount) {
		health = std::max (health - damageAmount, 0);
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
		weapons[currentWeaponIndex]->fire_weapon(scene_manager, ship_node->getPosition(), currentDirection);
	}

	boolean Enemy::inHostileRange(Ogre::Vector3 playerPos){
		float distance = GetMagnatude(GetVectorFromTwoPoints(playerPos, ship_node->getPosition()));
		if(distance <= PLAYER_HOSTILE_RADIUS ){
			return true;
		}
		return false;
	}

	boolean Enemy::inIntimidateRange(Ogre::Vector3 playerPos) {
		float distance = GetMagnatude(GetVectorFromTwoPoints(playerPos, ship_node->getPosition()));
		if(distance <= PLAYER_HOSTILE_RADIUS && distance > PLAYER_HOSTILE_RADIUS/4 ){
			return true;
		}
		return false;
	}

	boolean Enemy::inSpotRange(Ogre::Vector3 playerPos) {
		float distance = GetMagnatude(GetVectorFromTwoPoints(playerPos, ship_node->getPosition()));
		if(distance <= PLAYER_HOSTILE_RADIUS*2.5){
			return true;
		}
		return false;
	}

	boolean Enemy::isInArea(){
		float distance = GetMagnatude(GetVectorFromTwoPoints(Ogre::Vector3(0,0,0), ship_node->getPosition()));
		if(distance < NEBULA_RADIUS){
			return true;
		}
		return false;
	}

	// Checks To See If We Should Actually Run The Collision Detection Or Not
	boolean Enemy::should_run_collision(void) {
		return weapons[currentWeaponIndex]->getShotsFired().size() > 0;
	}

	boolean Enemy::enemyDead(void) {
		return health == 0;
	}

	int Enemy::getEnemyHealth(void) {
		return health;
	}

	float Enemy::getBoundingCircleRadius(void) {
		return boundingSphereRadius;
	}

	Ogre::Vector3 Enemy::getPosition(void){
		return ship_node->getPosition();
	}

	Ogre::Vector3 Enemy::getDirection(void){
		return currentDirection;
	}

	/*-------------------------------------------- Enemy Node Functions ----------------------------------*/
	void Enemy::createEnemyByNum (Ogre::SceneManager* scene_manager, Ogre::Vector3 pos, int num) {
		switch (num) {
		case 1: CreateEnemy1(scene_manager, pos); break;
		case 2: CreateEnemy2(scene_manager, pos); break;
		case 3: CreateEnemy3(scene_manager, pos); break;
		case 4: CreateEnemy4(scene_manager, pos); break;
		case 5: CreateEnemy5(scene_manager, pos); break;
		default: CreateEnemy1(scene_manager, pos); break;
		}
	}

	// Creates The Enemy When Called
	void Enemy::CreateEnemy1(Ogre::SceneManager* scene_manager, Ogre::Vector3 pos){
		Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();
		Ogre::Entity *entity;
		Ogre::Matrix4 transformations;
		Ogre::String part_name;

		entity = scene_manager->createEntity(entity_name, "cube.mesh");
		entity->setMaterialName("ShipTexture");

		ship_node = root_scene_node->createChildSceneNode(entity_name);
		ship_node->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(2.0, 2.0, 2.0));
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(0.0, 0.0, 0.0))) * transformations;
		AssignTransf(ship_node, transformations);

		part_name = "rightarm2";
		entity = scene_manager->createEntity(entity_name + part_name, "cube.mesh");  //mesh name on the right, entity on the left
		entity->setMaterialName("ShipTexture");
		Ogre::SceneNode* rightArm2 = ship_node->createChildSceneNode(entity_name + part_name);
		rightArm2->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(0.5, 0.5, 0.5));
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(-1.25, 0.0, 0.0))) * transformations;
		AssignTransf(rightArm2, transformations);

		part_name = "leftarm2";
		entity = scene_manager->createEntity(entity_name + part_name, "cube.mesh");  //mesh name on the right, entity on the left
		entity->setMaterialName("ShipTexture");
		Ogre::SceneNode* leftArm2 = ship_node->createChildSceneNode(entity_name + part_name);
		leftArm2->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(0.5, 0.5, 0.5));
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(1.25, 0.0, 0.0))) * transformations;
		AssignTransf(leftArm2, transformations);

		part_name = "rightcannon2";
		entity = scene_manager->createEntity(entity_name + part_name, "Cylinder");  //mesh name on the right, entity on the left
		Ogre::SceneNode* rightCannon = ship_node->createChildSceneNode(entity_name + part_name);
		rightCannon->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(0.25, 0.25, 0.25));
		transformations = Ogre::Matrix4(RotationMatrix(Ogre::Vector3(1.0, 0.0, 0.0), Ogre::Radian(Ogre::Math::PI/2.0))) * transformations;
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(-1.25, 0.0, -0.5))) * transformations;
		AssignTransf(rightCannon, transformations);

		part_name = "leftcannon2";
		entity = scene_manager->createEntity(entity_name + part_name, "Cylinder");  //mesh name on the right, entity on the left
		Ogre::SceneNode* leftCannon = ship_node->createChildSceneNode(entity_name + part_name);
		leftCannon->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(0.25, 0.25, 0.25));
		transformations = Ogre::Matrix4(RotationMatrix(Ogre::Vector3(1.0, 0.0, 0.0), Ogre::Radian(Ogre::Math::PI/2.0))) * transformations;
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(1.25, 0.0, -0.5))) * transformations;
		AssignTransf(leftCannon, transformations);

		boundingSphereRadius = 20;
		ship_node->scale(boundingSphereRadius / 10.0, boundingSphereRadius / 10.0, boundingSphereRadius / 10.0);
		ship_node->setPosition(pos);
	}

	// Our Second Enemy Type
	void Enemy::CreateEnemy2(Ogre::SceneManager* scene_manager, Ogre::Vector3 pos){

		Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();
		Ogre::Entity *entity;
		Ogre::Matrix4 transformations;
		Ogre::String part_name;

		part_name = "body3";
		entity = scene_manager->createEntity(entity_name + part_name, "Prism");  //mesh name on the right, entity on the left
		ship_node = root_scene_node->createChildSceneNode(entity_name + part_name);
		ship_node->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(2.0, 2.0, 2.0));
		transformations = Ogre::Matrix4(RotationMatrix(Ogre::Vector3(0.0, 0.0, -1.0), Ogre::Radian(Ogre::Math::PI))) * transformations;
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(0.0, 0.0, 1.0))) * transformations;
		AssignTransf(ship_node, transformations);

		part_name = "toparm3";
		entity = scene_manager->createEntity(entity_name + part_name, "Cylinder");  //mesh name on the right, entity on the left
		Ogre::SceneNode* topArm = ship_node->createChildSceneNode(entity_name + part_name);
		topArm->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(0.3, 0.20, 0.3));
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(-0.5, -1.0, 0.5))) * transformations;
		AssignTransf(topArm, transformations);

		part_name = "bottomleftarm3";
		entity = scene_manager->createEntity(entity_name + part_name, "Cylinder");  //mesh name on the right, entity on the left
		Ogre::SceneNode* bottomLeftArm = ship_node->createChildSceneNode(entity_name + part_name);
		bottomLeftArm->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(0.3, 0.20, 0.3));
		transformations = Ogre::Matrix4(RotationMatrix(Ogre::Vector3(0.0, 0.0, -1.0), Ogre::Radian(Ogre::Math::PI/2))) * transformations;
		transformations = Ogre::Matrix4(RotationMatrix(Ogre::Vector3(0.0, 0.0, -1.0), Ogre::Radian(Ogre::Math::PI/4))) * transformations;
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(-1.0, 0.5, 0.5))) * transformations;
		AssignTransf(bottomLeftArm, transformations);

		part_name = "bottomrightarm3";
		entity = scene_manager->createEntity(entity_name + part_name, "Cylinder");  //mesh name on the right, entity on the left
		Ogre::SceneNode* bottomRightArm = ship_node->createChildSceneNode(entity_name + part_name);
		bottomRightArm->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(0.3, 0.20, 0.3));
		transformations = Ogre::Matrix4(RotationMatrix(Ogre::Vector3(0.0, 0.0, 1.0), Ogre::Radian(Ogre::Math::PI/2))) * transformations;
		transformations = Ogre::Matrix4(RotationMatrix(Ogre::Vector3(0.0, 0.0, 1.0), Ogre::Radian(Ogre::Math::PI/4))) * transformations;
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(0.0, 0.5, 0.5))) * transformations;
		AssignTransf(bottomRightArm, transformations);

		part_name = "topgun3";
		entity = scene_manager->createEntity(entity_name + part_name, "Prism");  //mesh name on the right, entity on the left
		Ogre::SceneNode* topGun = ship_node->createChildSceneNode(entity_name + part_name);
		topGun->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(0.6, 0.6, 4.0));		
		transformations = Ogre::Matrix4(RotationMatrix(Ogre::Vector3(0.0, 0.0, 1.0), Ogre::Radian(Ogre::Math::PI))) * transformations;
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(-0.8, -1.9, -1.0))) * transformations;
		AssignTransf(topGun, transformations);

		part_name = "bottomleftgun3";
		entity = scene_manager->createEntity(entity_name + part_name, "Prism");  //mesh name on the right, entity on the left
		Ogre::SceneNode* bottomLeftGun = ship_node->createChildSceneNode(entity_name + part_name);
		bottomLeftGun->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(0.6, 0.6, 4.0));		
		transformations = Ogre::Matrix4(RotationMatrix(Ogre::Vector3(0.0, 0.0, 1.0), Ogre::Radian(Ogre::Math::PI/4))) * transformations;
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(-1.25, 1.25, -1.0))) * transformations;
		AssignTransf(bottomLeftGun, transformations);

		part_name = "bottomrightgun3";
		entity = scene_manager->createEntity(entity_name + part_name, "Prism");  //mesh name on the right, entity on the left
		Ogre::SceneNode* bottomRightGun = ship_node->createChildSceneNode(entity_name + part_name);
		bottomRightGun->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(0.6, 0.6, 4.0));		
		transformations = Ogre::Matrix4(RotationMatrix(Ogre::Vector3(0.0, 0.0, -1.0), Ogre::Radian(Ogre::Math::PI/4))) * transformations;
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(0.75, 0.75, -1.0))) * transformations;
		AssignTransf(bottomRightGun, transformations);

		boundingSphereRadius = 10;
		ship_node->setPosition(pos);
		ship_node->setScale (boundingSphereRadius, boundingSphereRadius, boundingSphereRadius);
	}


	// Our Third enemy type
	void Enemy::CreateEnemy3(Ogre::SceneManager* scene_manager, Ogre::Vector3 pos){

		Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();
		Ogre::Entity *entity;
		Ogre::Matrix4 transformations;
		Ogre::String part_name;

		part_name = "bodyone4";
		entity = scene_manager->createEntity(entity_name + part_name, "cube.mesh");
		entity->setMaterialName("ShipTexture");
		ship_node = root_scene_node->createChildSceneNode(entity_name + part_name);
		ship_node->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(2.0, 0.5, 2.0));
		AssignTransf(ship_node, transformations);

		part_name = "bodytwo4";
		entity = scene_manager->createEntity(entity_name + part_name, "cube.mesh");
		entity->setMaterialName("ShipTexture");
		Ogre::SceneNode* shipbody2 = ship_node->createChildSceneNode(entity_name + part_name);
		shipbody2->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(2.0, 0.5, 2.0));
		transformations = Ogre::Matrix4(RotationMatrix(Ogre::Vector3(0.0, 0.0, 1.0), Ogre::Radian(Ogre::Math::PI/2.0))) * transformations;

		AssignTransf(shipbody2, transformations);

		part_name = "rightwing4";
		entity = scene_manager->createEntity(entity_name + part_name, "Prism");
		Ogre::SceneNode* rightwing = ship_node->createChildSceneNode(entity_name + part_name);
		rightwing->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(0.5, 1.0, 2.4));
		transformations = Ogre::Matrix4(RotationMatrix(Ogre::Vector3(0.0, 0.0, 1.0), Ogre::Radian(Ogre::Math::PI/2.0))) * transformations;
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(-1.5, 0.25, -1.0))) * transformations;
		AssignTransf(rightwing, transformations);

		part_name = "leftwing4";
		entity = scene_manager->createEntity(entity_name + part_name, "Prism");
		Ogre::SceneNode* leftwing = ship_node->createChildSceneNode(entity_name + part_name);
		leftwing->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(0.5, 1.0, 2.4));
		transformations = Ogre::Matrix4(RotationMatrix(Ogre::Vector3(0.0, 0.0, -1.0), Ogre::Radian(Ogre::Math::PI/2.0))) * transformations;
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(1.5, -0.25, -1.0))) * transformations;
		AssignTransf(leftwing, transformations);

		part_name = "topwing4";
		entity = scene_manager->createEntity(entity_name + part_name, "Prism");
		Ogre::SceneNode* topwing = ship_node->createChildSceneNode(entity_name + part_name);
		topwing->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(0.5, 1.0, 2.4));
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(0.25, 1.5, -1.0))) * transformations;
		AssignTransf(topwing, transformations);

		part_name = "bottomwing4";
		entity = scene_manager->createEntity(entity_name + part_name, "Prism");
		Ogre::SceneNode* bottomwing = ship_node->createChildSceneNode(entity_name + part_name);
		bottomwing->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(0.5, 1.0, 2.4));
		transformations = Ogre::Matrix4(RotationMatrix(Ogre::Vector3(0.0, 0.0, -1.0), Ogre::Radian(Ogre::Math::PI))) * transformations;
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(-0.25, -1.5, -1.0))) * transformations;
		AssignTransf(bottomwing, transformations);

		part_name = "topgun4";
		entity = scene_manager->createEntity(entity_name + part_name, "Cylinder");
		Ogre::SceneNode* topgun = ship_node->createChildSceneNode(entity_name + part_name);
		topgun->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(0.5, 0.25, 0.5));
		transformations = Ogre::Matrix4(RotationMatrix(Ogre::Vector3(1.0, 0.0, 0.0), Ogre::Radian(Ogre::Math::PI/2))) * transformations;
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(0.0, 0.65, -1.0))) * transformations;
		AssignTransf(topgun, transformations);

		part_name = "bottomgun4";
		entity = scene_manager->createEntity(entity_name + part_name, "Cylinder");
		Ogre::SceneNode* bottomgun = ship_node->createChildSceneNode(entity_name + part_name);
		bottomgun->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(0.5, 0.25, 0.5));
		transformations = Ogre::Matrix4(RotationMatrix(Ogre::Vector3(1.0, 0.0, 0.0), Ogre::Radian(Ogre::Math::PI/2))) * transformations;
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(0.0, -0.65, -1.0))) * transformations;
		AssignTransf(bottomgun, transformations);

		part_name = "leftgun4";
		entity = scene_manager->createEntity(entity_name + part_name, "Cylinder");
		Ogre::SceneNode* leftgun = ship_node->createChildSceneNode(entity_name + part_name);
		leftgun->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(0.5, 0.25, 0.5));
		transformations = Ogre::Matrix4(RotationMatrix(Ogre::Vector3(1.0, 0.0, 0.0), Ogre::Radian(Ogre::Math::PI/2))) * transformations;
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(0.65, 0.0, -1.0))) * transformations;
		AssignTransf(leftgun, transformations);

		part_name = "rightgun4";
		entity = scene_manager->createEntity(entity_name + part_name, "Cylinder");
		Ogre::SceneNode* rightgun = ship_node->createChildSceneNode(entity_name + part_name);
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

	// Create Our Fourth Entity Mesh
	void Enemy::CreateEnemy4(Ogre::SceneManager* scene_manager, Ogre::Vector3 pos){
		Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();
		Ogre::Entity *entity;

		entity = scene_manager->createEntity(entity_name, "Enemy1");
		ship_node = root_scene_node->createChildSceneNode(entity_name);
		ship_node->attachObject(entity);

		boundingSphereRadius = 25;
		ship_node->setPosition(pos);
		ship_node->setScale (boundingSphereRadius, boundingSphereRadius, boundingSphereRadius);
	}

	// Create Our Fifth Type Of Enemy
	void Enemy::CreateEnemy5 (Ogre::SceneManager* manager, Ogre::Vector3 initalPosition) {
		Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();
		Ogre::Entity *entity = scene_manager->createEntity("test" + enemy_num++, "cube.mesh");
		entity->setMaterialName("ShipTexture");

		ship_node = root_scene_node->createChildSceneNode(entity_name);
		ship_node->attachObject(entity);

		boundingSphereRadius = 15;
		ship_node->setPosition(initalPosition);
		ship_node->setScale (boundingSphereRadius, boundingSphereRadius, boundingSphereRadius);
	}





	/*-------------------------------------------- Helper Functions --------------------------------------*/

	// Rotate enemy ship using an orbit transformation
	void Enemy::RotateShip(Ogre::Vector3 target){
		ship_node->lookAt(target, Ogre::Node::TS_WORLD, initialDir);
	}

	Ogre::Vector3 Enemy::toOrigin(void){
		return GetVectorFromTwoPoints(Ogre::Vector3(0,0,0), ship_node->getPosition());
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