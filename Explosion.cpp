#include "Explosion.h"

namespace Explosion_Space {

	static int explosion_num = 0;

	Explosion::Explosion (void) {
	}

	Explosion::~Explosion (void) {
		deleteExplosionFromScene();
	}

	Explosion::Explosion (Ogre::SceneManager* manager, Ogre::Vector3 pos) {
		initialPos = pos;
		scene_manager = manager;
		explosion_name = "explosion" + Ogre::StringConverter::toString(explosion_num++);

		currentExplosionTimer = 0;
		explosionNode = nullptr;

		// BA BOOM!
		detonate();
	}

	// Deletes Our Explosion When Called
	void Explosion::deleteExplosionFromScene(void) {
		if (explosionNode) {
			Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

			// Destory Our Entities
			Ogre::SceneNode::ObjectIterator it = explosionNode->getAttachedObjectIterator();
			while (it.hasMoreElements())
			{
				Ogre::Entity* entity = static_cast<Ogre::Entity*>(it.getNext());
				scene_manager->destroyEntity(entity->getName());
			}

			// Finish Deleting Our Node
			root_scene_node->removeAndDestroyChild(explosionNode->getName());
			explosionNode = nullptr;
		}
	}

	// KABOOM :)
	void Explosion::detonate (void) {
		/* Retrieve scene manager and root scne node */
		Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

		/* Create entity */
		Ogre::Entity* entity = scene_manager->createEntity("particle.mesh");

		/* Apply a material to the entity */
		entity->setMaterialName("ParticleMaterial");

		/* Create a scene node for the entity */
		/* The scene node keeps track of the entity's position */
		explosionNode = root_scene_node->createChildSceneNode(explosion_name);
		explosionNode->attachObject(entity);

		/* Scale the entity */
		explosionNode->setPosition (initialPos);
	}

	// Updates Our Explosion With The New Time
	void Explosion::updateMaterialWithTimer(Ogre::Real time) {
		if (explosionNode) {
			currentExplosionTimer += time;
			Ogre::MaterialPtr ptr = Ogre::MaterialManager::getSingleton().getByName("ParticleMaterial");
			ptr->getBestTechnique()->getPass(0)->getVertexProgramParameters()->setNamedConstant("timer", currentExplosionTimer);
		}
	}

	// Returns The Position Of The Explosion, Whil
	Ogre::Vector3 Explosion::getPosition (void) {
		if (explosionNode) {
			return explosionNode->getPosition();
		} else {
			return Ogre::Vector3 (0.0f,0.0f, 0.0f);
		}
	}

	float Explosion::getExplosionTimer(void) {
		return currentExplosionTimer;
	}

	// Advances Our Explosion In Time
	void Explosion::advance (Ogre::Real timeSinceLastFrame) {
		updateMaterialWithTimer(timeSinceLastFrame);

		if (currentExplosionTimer > EXPLOSION_LIFE_SPAN) {
			deleteExplosionFromScene();
		}
	}

	boolean Explosion::shouldDeleteExplosion(void) {
		return currentExplosionTimer > EXPLOSION_LIFE_SPAN;
	}
}