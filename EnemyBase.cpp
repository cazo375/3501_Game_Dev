#include "Enemybase.h"

// Enemy Implementation
namespace Enemy_Space {

	Enemy::Enemy (Ogre::SceneManager* scene_manager) {
		Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();
		Ogre::Entity *entity = scene_manager->createEntity("test", "Icosahedron");
	}

	Enemy::~Enemy (void) {
	}
}