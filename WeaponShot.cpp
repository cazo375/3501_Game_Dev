#include "WeaponShot.h"

namespace Weapon_Shot_Space {

	static int playerShot = 0;

	Weapon_Shot::Weapon_Shot (void) {
	}

	// Constructor For The Weapons Shot
	Weapon_Shot::Weapon_Shot(Ogre::Vector3 dir, Ogre::String shotName, int damage_amount) {
		lifeCounter = 0;
		damageAmount = damage_amount;
		direction = dir;
		entity_name = shotName + "shot" + Ogre::StringConverter::toString(playerShot++);
		weapon_mesh = "cube.mesh";
		weapon_shot_node = nullptr;
	}

	Weapon_Shot::~Weapon_Shot(void) {
		destoryFiredWeapon();
	}

	void Weapon_Shot::createEntity (Ogre::SceneManager* manager, Ogre::Vector3 position) {
		scene_manager = manager;

		// Create And Place The Obhect
		Ogre::Entity* entity = scene_manager->createEntity(entity_name, weapon_mesh);
		Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();
		weapon_shot_node = root_scene_node->createChildSceneNode(entity_name);
		weapon_shot_node->attachObject(entity);
		weapon_shot_node->setPosition(position);
		weapon_shot_node->setScale (0.50f, 0.50f, 0.5f);
	}

	// Moves Our Lazer If It Is Defined
	void Weapon_Shot::moveShot(Ogre::Real time) {
		if (weapon_shot_node) {
			weapon_shot_node->translate(direction * LAZER_THRUST);
			lifeCounter += time;

			if (lifeCounter >= LAZER_LIFE_SPAN) {
				destoryFiredWeapon();
			}
		}
	}

	// Destorys the lazer when called
	void Weapon_Shot::destoryFiredWeapon (void) {
		if (weapon_shot_node) {
			Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();
			root_scene_node->removeAndDestroyChild(weapon_shot_node->getName());
			scene_manager->destroyEntity(entity_name);
			weapon_shot_node = 0;
		}
	}

	boolean Weapon_Shot::shouldDestoryShot (void) {
		return lifeCounter >= LAZER_LIFE_SPAN;
	}

	Ogre::Vector3 Weapon_Shot::getPosition(void) {
		return weapon_shot_node->getPosition();
	}

	Ogre::Vector3 Weapon_Shot::getDirection(void) {
		return direction;
	}

	float Weapon_Shot::getDamageAmount(void) {
		return damageAmount;
	}

	/*------------------------------------------------- Exposive Shell ----------------------------------*/
	Explosive_Shot::Explosive_Shot (Ogre::Vector3 dir, Ogre::String shotName, int damage_amount) : Weapon_Shot (dir, shotName, damage_amount){
		exploded = false;
		explosion = nullptr;
		explosion_timer = 0;
	}

	Explosive_Shot::~Explosive_Shot(void) {
		if (explosion) {
			delete explosion;
			explosion = nullptr;
		}
	}

	boolean Explosive_Shot::shouldDestoryShot (void) {
		return lifeCounter >= LAZER_LIFE_SPAN + EXPLOSION_LIFE_SPAN;
	}

	void Explosive_Shot::moveShot(Ogre::Real time) {
		if (weapon_shot_node) {
			weapon_shot_node->translate(direction * LAZER_THRUST);
			lifeCounter += time;

			// If The Shot Has Traveled
			if (lifeCounter > EXPLOSION_DELAY && !exploded) {
				exploded = true;
				explosion_timer = 0;
				explosion = new Explosion_Space::Explosion(scene_manager, weapon_shot_node->getPosition());
			}
		}

		// If The Explosion Exists Then Progress
		if (explosion) {
			explosion->advance(time);
		}
	}
}