/*
Our Generic Weapon Class. Holds The Data That Is Relevant To The Weapons.
*/

#include "OGRE/OgreRoot.h"
#include "OGRE/OgreRenderSystem.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreWindowEventUtilities.h"
#include "OGRE/OgreManualObject.h"
#include "OGRE/OgreEntity.h"
#include "OIS/OIS.h"
#include <vector>

#define LAZER_THRUST 6.0f
#define LAZER_LIFE_SPAN 12.0f

namespace Weapon_Space {
	class Weapon_Shot {
	public:
		Weapon_Shot(void);
		Weapon_Shot(Ogre::SceneManager* manager, Ogre::Vector3 position, Ogre::Vector3 direction);
		~Weapon_Shot(void);

		void moveShot (void);
		void destoryFiredWeapon (void);
		Ogre::Vector3 getPosition (void);
		Ogre::Vector3 getDirection (void);
		boolean shouldDestoryShot (void);

	protected:
		void createEntity (Ogre::Vector3 pos);

		// Variables For Creation
		Ogre::String weapon_mesh;
		Ogre::String entity_name;
		Ogre::SceneNode* weapon_shot_node;
		Ogre::SceneManager* scene_manager;

		// Values That Need To Be Know For Where The Object Is Going
		Ogre::Vector3 direction;
		Ogre::Quaternion ori;

		// Values For Movement
		float lifeCounter;
	};
}