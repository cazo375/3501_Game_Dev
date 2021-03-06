// Collision Manager For Our Small Game
#include <vector>
#include "OGRE/OgreRoot.h"
#include "OGRE/OgreRenderSystem.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreWindowEventUtilities.h"
#include "OGRE/OgreManualObject.h"
#include "OGRE/OgreEntity.h"
#include "OIS/OIS.h"

// Name Space Definition
namespace Collision_Manager {
	
	// Base Class For This Object
	class CollisionManager {
	public:
		static bool runRaySphereCollision (Ogre::Vector3 rayStart, Ogre::Vector3 rayDir, Ogre::Vector3 spherePosition, float sphereRadius);
				static bool runBoundingSphereCollision (Ogre::Vector3 obj1, Ogre::Vector3 obj2, float radius1, float radius2);
		static bool runMissleSphereCollision (Ogre::Vector3 missleStart, Ogre::Vector3 missleDir, float length, Ogre::Vector3 spherePositon, float sphereRadius);
	};
}