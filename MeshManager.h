// Collision Manager For Our Small Game
#include <vector>
#include "OGRE/OgreRoot.h"
#include "OGRE/OgreRenderSystem.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreWindowEventUtilities.h"
#include "OGRE/OgreManualObject.h"
#include "OGRE/OgreEntity.h"
#include "OIS/OIS.h"

namespace Mesh_Manager_Space {
	
	// Base Class For This Object
	class Mesh_Manager {
	public:
		static void createCubeMesh(Ogre::SceneManager* manager);
		static void createIcohedron (Ogre::SceneManager* manager);
		static void createSphere (Ogre::SceneManager* manager);
		static void createReverseSphere (Ogre::SceneManager* manager);
		static void createEnemy1(Ogre::SceneManager* scene_manager);
		static void createCylinder(Ogre::SceneManager* scene_manager);
		static void createPrism(Ogre::SceneManager* scene_manager);
		static void buildAllMeshes(Ogre::SceneManager* manager);

	};
}