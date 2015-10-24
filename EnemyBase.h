/*
	Our Generic Enemy Class. Holds The Data That Is Relevant To The Enemy.
*/

#include "OGRE/OgreRoot.h"
#include "OGRE/OgreRenderSystem.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreWindowEventUtilities.h"
#include "OGRE/OgreManualObject.h"
#include "OGRE/OgreEntity.h"
#include "OIS/OIS.h"

namespace Enemy_Space {

	class Enemy {
	public:
		Enemy (Ogre::SceneManager* scene_manager);
		~Enemy (void);
	protected:
	};
}