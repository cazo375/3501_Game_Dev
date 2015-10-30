#include "Level_Manager.h"

// Method Defintions For Our Level Manager
namespace Level_Manager_Space {

	Level_Manager::Level_Manager(void) : currentLevel(-1), level_switch_counter(0) {
		createAllLevels();
	}

	Level_Manager::~Level_Manager(void) {
	}

	// Level Progression Stuff
	int Level_Manager::getCurrentLevel (void) {
		return currentLevel % levels.size();
	}

	Level_Space::Level* Level_Manager::getCurrentLevelObj (void) {
		return levels[currentLevel % levels.size()];
	}

	// Advances Our Current Level A Frame When Called 
	void Level_Manager::advanceCurrentLevel (void) {
		levels[currentLevel % levels.size()]->advance();
	}

	// Cylces The Levels When Called
	void Level_Manager::cycleNextLevel (Ogre::SceneManager* scene_manager) {
		int test = levels.size();
		if (currentLevel + 1 < test) {
			if (currentLevel >= 0) {
				levels[currentLevel % levels.size()]->destoryLevel(scene_manager);
			}

			currentLevel++;

			level_switch_counter = 0;
			levels[currentLevel % levels.size()]->createLevel(scene_manager);
		}
	}

	// Creates Our Levels When Called
	void Level_Manager::createAllLevels (void) {
		levels.push_back(new Level_Space::Level(0));
		levels.push_back(new Level_Space::Level(1));
	}

	// Checks To See If We Can Switch Levels
	boolean Level_Manager::canSwitchLevels (void) {
		return level_switch_counter >= TIME_GAP_LEVEL_SWITCH;
	}

	// Increments Our Timer When Called
	void Level_Manager::incrementLevelTicker (void) {
		if (level_switch_counter < TIME_GAP_LEVEL_SWITCH) {
			level_switch_counter++;
		}
	}
}