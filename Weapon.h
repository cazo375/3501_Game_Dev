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

#define SCATTER_RADIUS 0.01f
#define NUM_OF_SCATTER_PELLETS 5

namespace Weapon_Space {

	class BaseWeapon {
	public:
		BaseWeapon(void);
		~BaseWeapon(void);

		void advance(Ogre::Real);
		boolean getOrientationNeeded(void);
		Ogre::String getWeaponName(void);
		std::vector<Weapon_Shot_Space::Weapon_Shot*> getShotsFired(void);

		// Abstract Function
		virtual void fire_weapon(Ogre::SceneManager*, Ogre::Vector3, Ogre::Vector3);
		virtual void fire_weapon(Ogre::SceneManager*, Ogre::Vector3, Ogre::Vector3, Ogre::Quaternion);
	protected:
		// Weapon Values
		int weapon_damage;
		int fireRate;
		int timeRemainingBeforeShot;
		Ogre::String weapon_name;
		Ogre::String owner_object;
		boolean needsOrientation;

		// Shots Fired By This Weapon
		std::vector<Weapon_Shot_Space::Weapon_Shot*> shotsFired;

		// Functions To Inherit
		virtual void initialize_weapon(void) = 0;
	};

	// Basic Lazer Object
	class Lazer : public BaseWeapon {
	public:
		Lazer(void);
		Lazer (Ogre::String);
		~Lazer(void);

		void initialize_weapon(void);
	};

	// Basic Bomb Weapon
	class Bomb : public BaseWeapon {
	public:
		Bomb(void);
		Bomb (Ogre::String);
		~Bomb(void);

		void initialize_weapon(void);
		void fire_weapon(Ogre::SceneManager*, Ogre::Vector3, Ogre::Vector3);
	};

	// Basic Bomb Weapon
	class Scatter_Shot : public BaseWeapon {
	public:
		Scatter_Shot (void);
		Scatter_Shot (Ogre::String);
		~Scatter_Shot (void);

		void initialize_weapon(void);
		void fire_weapon(Ogre::SceneManager*, Ogre::Vector3, Ogre::Vector3, Ogre::Quaternion);
	};
}

#endif