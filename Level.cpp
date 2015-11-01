#include "Level.h"

namespace Level_Space {

	Level::Level(void) {
		levelNum = -1;
	}

	// Constructor For Level
	Level::Level(int levelNum) : levelNum(levelNum){
	}

	Level::~Level(void) {
	}

	// Advances Our Level A Frame
	void Level::advance (void) {
		std::vector<Planet_Space::Planet*>::iterator iter = planets.begin();
		std::vector<Planet_Space::Planet*>::iterator iter_end = planets.end();
		for (; iter != iter_end; iter++ ) {
			(*iter)->advance();
		}

		std::vector<Enemy_Space::Enemy*>::iterator enemy_iter = enemies.begin();
		std::vector<Enemy_Space::Enemy*>::iterator enemy_iter_end = enemies.end();
		for (; enemy_iter != enemy_iter_end; enemy_iter++ ) {
			(*enemy_iter)->advance();
		}
	}

	// Creates Our Level With All It's Entites
	void Level::createLevel (Ogre::SceneManager* scene_manager) {
		switch (levelNum) {
		case 0: 
			planets.push_back(new Planet_Space::Planet(scene_manager, VENUS)); 
			enemies.push_back(new Enemy_Space::Enemy (scene_manager, Ogre::Vector3 (0.0f, 0.0f, 750.0f)));
			break;
		case 1:
			planets.push_back(new Planet_Space::Planet(scene_manager, EARTH));
			break;
		} 
	}

	// Destroys Our Level With All Of It's Contents
	void Level::destoryLevel (Ogre::SceneManager* scene_manager) {
		std::vector<Planet_Space::Planet*>::iterator iter = planets.begin();
		std::vector<Planet_Space::Planet*>::iterator iter_end = planets.end();
		for (; iter != iter_end; iter++ ) {
			(*iter)->destoryPlanet(scene_manager);
		}

		std::vector<Enemy_Space::Enemy*>::iterator enemy_iter = enemies.begin();
		std::vector<Enemy_Space::Enemy*>::iterator enemy_iter_end = enemies.end();
		for (; enemy_iter != enemy_iter_end; enemy_iter++ ) {
			(*enemy_iter)->destoryEnemy();
		}
	}

	std::vector<Planet_Space::Planet*> Level::getPlanets(void) {
		return planets;
	}

	std::vector<Enemy_Space::Enemy*> Level::getEnemies (void) {
		return enemies;
	}
}