#include "OGRE/OgreRoot.h"
#include "OGRE/OgreRenderSystem.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreWindowEventUtilities.h"
#include "OGRE/OgreManualObject.h"
#include "OGRE/OgreEntity.h"
#include "OIS/OIS.h"
#include <vector>
#include "WeaponShot.h"

#ifndef WEAPON_H
#define WEAPON_H

namespace Weapon_Space {

	class BaseWeapon {
	public:
		BaseWeapon(void);
		~BaseWeapon(void);

		void advance(Ogre::Real);
		Ogre::String getWeaponName(void);
		std::vector<Weapon_Shot_Space::Weapon_Shot*> getShotsFired(void);

		// Abstract Function
		virtual void fire_weapon(Ogre::SceneManager*, Ogre::Vector3, Ogre::Vector3);
	protected:
		// Weapon Values
		int weapon_damage;
		int fireRate;
		int timeRemainingBeforeShot;
		Ogre::String weapon_name;
		Ogre::String owner_object;

		// Shots Fired By This Weapon
		std::vector<Weapon_Shot_Space::Weapon_Shot*> shotsFired;
	};

	// Basic Lazer Object
	class Lazer : public BaseWeapon {
	public:
		Lazer(void);
		Lazer (Ogre::String);
		~Lazer(void);
	};

	// Basic Bomb Weapon
	class Bomb : public BaseWeapon {
	public:
		Bomb(void);
		Bomb (Ogre::String);
		~Bomb(void);

		void fire_weapon(Ogre::SceneManager*, Ogre::Vector3, Ogre::Vector3);
	};
}

#endif