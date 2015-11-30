#include <vector>
#include "OGRE/OgreRoot.h"
#include "OGRE/OgreRenderSystem.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreWindowEventUtilities.h"
#include "OGRE/OgreManualObject.h"
#include "OGRE/OgreEntity.h"
#include "OIS/OIS.h"
#include "Level.h"

#ifndef LEVEL_MANAGER_H

#define LEVEL_MANAGER_H
#define TIME_GAP_LEVEL_SWITCH 60
#define MAX_LEVEL_AMOUNT 10

namespace Level_Manager_Space {
	class Level_Manager  {
	public:
		Level_Manager(void);
		~Level_Manager(void);

		// Level Progression Stuff
		int getCurrentLevel (void);

		void createAllLevels (void);
		void advanceCurrentLevel (Player_Space::Player* player, Ogre::Real time);
		void cycleNextLevel (Ogre::SceneManager* scene_manager);
		void incrementLevelTicker (void);

		boolean canSwitchLevels (void);
		Level_Space::Level* getCurrentLevelObj (void);

	private:
		int currentLevel;
		long level_switch_counter;
		std::vector<Level_Space::Level*> levels;
	};
}

#endif