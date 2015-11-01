#include "WeaponBase.h"

namespace Weapon_Space {

	Weapon_Shot::Weapon_Shot (void) {
	}

	// Constructor For The Weapons Shot
	Weapon_Shot::Weapon_Shot(Ogre::SceneManager* manager, Ogre::Vector3 position, Ogre::Vector3 dir) {
		scene_manager = manager;
		lifeCounter = 0;
		direction = dir;
		entity_name = "shot";
		weapon_mesh = "cube.mesh";

		createEntity(position);
	}

	Weapon_Shot::~Weapon_Shot(void) {
	}

	void Weapon_Shot::createEntity (Ogre::Vector3 position) {
		// Create And Place The Obhect
		Ogre::Entity *entity = scene_manager->createEntity(entity_name, weapon_mesh);
		Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();
		weapon_shot_node = root_scene_node->createChildSceneNode(entity_name);
		weapon_shot_node->attachObject(entity);
		weapon_shot_node->setPosition(position);
		weapon_shot_node->setScale (0.50f, 0.50f, 0.5f);
	}

	// Moves Our Lazer If It Is Defined
	void Weapon_Shot::moveShot(void) {
		weapon_shot_node->translate(direction * LAZER_THRUST);
		lifeCounter++;

		if (lifeCounter >= LAZER_LIFE_SPAN) {
			destoryFiredWeapon();
		}
	}

	// Destorys the lazer when called
	void Weapon_Shot::destoryFiredWeapon (void) {
		Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();
		root_scene_node->removeAndDestroyChild(weapon_shot_node->getName());
		scene_manager->destroyEntity(entity_name);
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
}