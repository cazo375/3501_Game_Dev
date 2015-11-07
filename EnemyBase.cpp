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
	Enemy::Enemy (Ogre::SceneManager* scene_man, Ogre::Vector3 initalPosition) : scene_manager(scene_man), alive(true), enemyHealth(10), boundingSphereRadius(4), original_position(initalPosition){
		shot = nullptr;
		currentDirection = Ogre::Vector3(0.0,0.0,0.0);
		createEnemy(scene_man, initalPosition);
		buildPointGraph();
	}

	// Creates The Enemy When Called
	void Enemy::createEnemy (Ogre::SceneManager* manager, Ogre::Vector3 initalPosition) {
		Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();
		Ogre::Entity *entity = scene_manager->createEntity("test" + enemy_num++, "cube.mesh");

		entity_name = "enemy" + enemy_num;
		ship_node = root_scene_node->createChildSceneNode(entity_name);
		ship_node->attachObject(entity);

		ship_node->setPosition(Ogre::Vector3 (initalPosition.x, initalPosition.y, initalPosition.z - 15));
		ship_node->setScale (boundingSphereRadius, boundingSphereRadius, boundingSphereRadius);
	}

	// Builds The Graph That This Enemy Will Use To Move
	void Enemy::buildPointGraph(void) {
		currentPathIndex = 0;
		pathPoints.push_back(Ogre::Vector3 (original_position.x + 10, original_position.y, original_position.z - 15));
		pathPoints.push_back(Ogre::Vector3 (original_position.x - 10, original_position.y, original_position.z - 15));
	}

	// Moves The Enemey One Frame In It's Cycle
	void Enemy::advance(void) {
		if (ship_node) {
			currentDirection = pathPoints[currentPathIndex  % pathPoints.size()] - ship_node->getPosition();
			currentDirection.normalise();
			ship_node->translate(currentDirection * ENEMY_MOVE_SPEED);

			// Call The Graph Cycler 
			cycleGraphPointIfNeeded();

			// Finally Move Our Lazer If It Exists
			moveLazer();
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
}