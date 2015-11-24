#include <vector>
#include "OGRE/OgreRoot.h"
#include "OGRE/OgreRenderSystem.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreWindowEventUtilities.h"
#include "OGRE/OgreManualObject.h"
#include "OGRE/OgreEntity.h"
#include "OIS/OIS.h"
#include "WeaponShot.h"
#include "Weapon.h"

#ifndef Ship_h
#define Ship_h

namespace Ship_Space {
	class Ship {
	public:
		// Collision Mechancis
		Ogre::Vector3 getPosition(void);

		// Projectile Mechanics
		void moveLazer (void);
		std::vector <Weapon_Shot_Space::Weapon_Shot*> getCurrentShots(void);

		// Abstract Methods
		virtual float getBoundingCircleRadius(void) = 0;
		virtual void fireShot (void) = 0;

	protected:
		// Weapons Attached To This Ship
		int currentWeaponIndex;
		std::vector<Weapon_Space::BaseWeapon*> weapons;
		Weapon_Shot_Space::Weapon_Shot* shot;

		// Entity Variables
		Ogre::SceneNode* ship_node;
	};
}

#endif