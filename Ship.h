#include <vector>
#include "OGRE/OgreRoot.h"
#include "OGRE/OgreRenderSystem.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreWindowEventUtilities.h"
#include "OGRE/OgreManualObject.h"
#include "OGRE/OgreEntity.h"
#include "OIS/OIS.h"
#include "WeaponBase.h"

#define LAZER_THRUST 6.0f
#define LAZER_LIFE_SPAN 12.0f

#ifndef Ship_h
#define Ship_h

namespace Ship_Space {
	class Ship {
	public:
		// Collision Mechancis
		Ogre::Vector3 getPosition(void);

		// Projectile Mechanics
		void moveLazer (void);
		Weapon_Space::Weapon_Shot* getCurrentShot(void);

		// Abstract Methods
		virtual float getBoundingCircleRadius(void) = 0;
		virtual void fireShot (void) = 0;

	protected:
		Weapon_Space::Weapon_Shot* shot;
		Ogre::SceneNode* ship_node;
	};
}

#endif