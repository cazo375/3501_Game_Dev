#include "Asteroid.h"

namespace Asteroid_Space {

	static int asteriod_num = 0;

	Asteroid::Asteroid(void) {
	}

	Asteroid::Asteroid(Ogre::SceneManager* manager) {
			scene_manager = manager;
			asteroidNode = nullptr;

			ori = Ogre::Quaternion(1.0f, 3.14*(rand() % 1000) / 1000.0f, 3.14*(rand() % 1000) / 1000.0f, 3.14*(rand() % 1000) / 1000.0f);
			lm = Ogre::Quaternion(1.0f, 0.005*3.14*(rand() % 1000) / 1000.0f, 0.005*3.14*(rand() % 1000) / 1000.0f, 0.005*3.14*(rand() % 1000) / 1000.0f);
			drift = Ogre::Vector3(((double) rand() / RAND_MAX)*0.2, ((double) rand() / RAND_MAX)*0.2, ((double) rand() / RAND_MAX)*0.2);
			asteroidName = "asteroid" + Ogre::StringConverter::toString(asteriod_num++);

			Ogre::Vector3 pos = Ogre::Vector3(-300 + 600 * (rand() % 1000) / 1000.0f, -300 + 600 * (rand() % 1000) / 1000.0f, 600 * (rand() % 1000) / 1000.0f);
			createAsteroid(pos);
	}

	Asteroid::~Asteroid(void) {
		if (asteroidNode) {
			deleteAsteroidFromScene();
		}
	}

	// Advances Our Asteroid If It Is Defined
	void Asteroid::advance(void) {
		if (asteroidNode) {
			ori = lm * ori;
			asteroidNode->setOrientation(ori);
		}
	}

	// Deletes Our Asteroid When Called
	void Asteroid::deleteAsteroidFromScene (void) {
		if (asteroidNode) {
			Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

			// Destory Our Entities
			Ogre::SceneNode::ObjectIterator it = asteroidNode->getAttachedObjectIterator();
			while (it.hasMoreElements())
			{
				Ogre::Entity* entity = static_cast<Ogre::Entity*>(it.getNext());
				scene_manager->destroyEntity(entity->getName());
			}

			// Finish Deleting Our Node
			root_scene_node->removeAndDestroyChild(asteroidNode->getName());
			asteroidNode = nullptr;
		}
	}

	// Creates Our Asteroid When Called
	void Asteroid::createAsteroid (Ogre::Vector3 pos) {
		Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

		/* Create entity */
		Ogre::Entity *entity = scene_manager->createEntity(asteroidName, "Icosahedron");
		entity->setMaterialName("ShinyBlueMaterial");

		/* Create a scene node for the entity */
		/* The scene node keeps track of the entity's position */
		asteroidNode = root_scene_node->createChildSceneNode(asteroidName);
		asteroidNode->attachObject(entity);
		asteroidNode->setPosition(pos);
	}

	Ogre::Vector3 Asteroid::getPosition (void) {
		return asteroidNode->getPosition();
	}

}
