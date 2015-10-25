#include "Enemybase.h"

// Enemy Implementation
namespace Enemy_Space {

	static int enemy_num;

	Enemy::Enemy(void) {
	}

	Enemy::~Enemy (void) {
	}

	// Enemy Constructor
	Enemy::Enemy (Ogre::SceneManager* scene_man, Ogre::Vector3 initalPosition) : scene_manager(scene_man) {
		Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();
		Ogre::Entity *entity = scene_manager->createEntity("test" + enemy_num++, "cube.mesh");

		entity_name = "enemy" + enemy_num;
		enemy_node = root_scene_node->createChildSceneNode(entity_name);
		enemy_node->attachObject(entity);
		original_position = initalPosition;

		enemy_node->setPosition(Ogre::Vector3 (initalPosition.x, initalPosition.y, initalPosition.z - 15));

		buildPointGraph();
	}

	// Builds The Graph That This Enemy Will Use To Move
	void Enemy::buildPointGraph(void) {
		currentPathIndex = 0;
		pathPoints.push_back(Ogre::Vector3 (original_position.x + 10, original_position.y, original_position.z - 15));
		pathPoints.push_back(Ogre::Vector3 (original_position.x - 10, original_position.y, original_position.z - 15));
	}

	// Moves The Enemey One Frame In It's Cycle
	void Enemy::advance(void) {
		if (enemy_node) {
			Ogre::Vector3 directionVector = pathPoints[currentPathIndex  % pathPoints.size()] - enemy_node->getPosition();
			directionVector.normalise();
			enemy_node->translate(directionVector * ENEMY_MOVE_SPEED);

			if (enemy_node->getPosition().squaredDistance(pathPoints[currentPathIndex % pathPoints.size()]) < NODE_MIN_RADIUS) {
				currentPathIndex++;

				if (currentPathIndex == 2) {
					destoryEnemy();
				}
			}
		}
	}

	// Destories The Enemy And Removes Them From The Scene 
	void Enemy::destoryEnemy(void) {
		Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();
		root_scene_node->removeAndDestroyChild(enemy_node->getName());
		scene_manager->destroyEntity(entity_name);
		enemy_node = 0;
	}
}