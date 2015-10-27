#include <vector>
#include "OGRE/OgreRoot.h"
#include "OGRE/OgreRenderSystem.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreWindowEventUtilities.h"
#include "OGRE/OgreManualObject.h"
#include "OGRE/OgreEntity.h"
#include "OIS/OIS.h"

#include "EnemyBase.h"
#include "Planet.h"

// The Actual Space For Levels... They Will Be Cycled Through The Level_Manager
namespace Level_Space {
	class Level
	{
	public:
				Level(void);
		Level(int levelNum);
		~Level(void);

		void advance (void);
		void createLevel (Ogre::SceneManager* scene_manager);
		void destoryLevel (Ogre::SceneManager* scene_manager);
	private:
		int levelNum;
		std::vector<Enemy_Space::Enemy> enemies;
		std::vector<Planet_Space::Planet> planets;
	};
}