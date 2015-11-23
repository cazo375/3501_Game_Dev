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
	void Level::advance (Player_Space::Player* player) {
		std::vector<Planet_Space::Planet*>::iterator iter = planets.begin();
		std::vector<Planet_Space::Planet*>::iterator iter_end = planets.end();
		for (; iter != iter_end; iter++ ) {
			(*iter)->advance();
		}

		std::vector<Enemy_Space::Enemy*>::iterator enemy_iter = enemies.begin();
		std::vector<Enemy_Space::Enemy*>::iterator enemy_iter_end = enemies.end();
		for (; enemy_iter != enemy_iter_end; enemy_iter++ ) {
			(*enemy_iter)->advance(player);
		}

		for (int i = 0; i < asteroids.size(); i++) {
			asteroids[i]->advance();
		}
	}

	// Creates Our Level With All It's Entites
	void Level::createLevel (Ogre::SceneManager* scene_manager) {
		switch (levelNum) {
		case 0: 
			planets.push_back(new Planet_Space::Planet(scene_manager, VENUS, Ogre::Vector3 (0.0f, 0.0f, 0.0f))); 
			planets.push_back(new Planet_Space::Planet(scene_manager, NEPTUNE, Ogre::Vector3 (-150.0f, 150.0f, 500.0f))); 
			planets.push_back(new Planet_Space::Planet(scene_manager, JUPITER, Ogre::Vector3 (100.0f, -75.0f, -320.0f))); 
			enemies.push_back(new Enemy_Space::Enemy (scene_manager, "John", Ogre::Vector3 (0.0f, 0.0f, 550.0f), 4));
			enemies.push_back(new Enemy_Space::Enemy (scene_manager, "Tom", Ogre::Vector3 (200.0f, 100.0f, 650.0f), 0));
			//enemies.push_back(new Enemy_Space::Enemy (scene_manager, Ogre::Vector3 (0.0f, 10.0f, 550.0f)));
			//enemies.push_back(new Enemy_Space::Enemy (scene_manager, Ogre::Vector3 (0.0f, -5.0f, 550.0f)));
			//enemies.push_back(new Enemy_Space::Enemy (scene_manager, Ogre::Vector3 (0.0f, -10.0f, 550.0f)));
			break;
		case 1:
			planets.push_back(new Planet_Space::Planet(scene_manager, EARTH));
			planets.push_back(new Planet_Space::Planet(scene_manager, JUPITER, Ogre::Vector3 (150.0f, 300.0f, -200.0f))); 
			break;
		} 

		createAsteroidField(scene_manager);
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

		for (int i = 0;i < asteroids.size(); i++) {
			delete asteroids[i];
		}
	}

	// Removes An Enemey At An Index
	void Level::destoryEnemyAt (int index) {
		delete enemies[index];
		enemies.erase(enemies.begin() + index);
	}

	void Level::destoryAsteroidAt (int index) {
		delete asteroids[index];
		asteroids.erase(asteroids.begin() + index);
	}

	// Populate Our Asteroid Field
	void Level::createAsteroidField (Ogre::SceneManager* scene_manager) {
		for (int i = 0; i < NUM_ASTEROIDS_PER_LEVEL; i++) {
			asteroids.push_back (new Asteroid_Space::Asteroid (scene_manager));
		}
	}

	std::vector<Planet_Space::Planet*> Level::getPlanets(void) {
		return planets;
	}

	std::vector<Enemy_Space::Enemy*> Level::getEnemies (void) {
		return enemies;
	}

	std::vector<Asteroid_Space::Asteroid*> Level::getAsteroids (void) {
		return asteroids;
	}
}