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
	void Level::advance (Player_Space::Player* player, Ogre::Real time) {
		for (int i = 0; i < planets.size(); i++ ) {
			planets[i]->advance();
		}

		for (int i = 0; i < enemies.size(); i++ ) {
			enemies[i]->advance(player, time);
		}

		for (int i = 0; i < asteroids.size(); i++) {
			asteroids[i]->advance();
		}
	}

	// Creates Our Level With All It's Entites
	void Level::createLevel (Ogre::SceneManager* scene_manager) {
		switch (levelNum) {
		case 0:  
			planets.push_back(new Planet_Space::Planet(scene_manager, NEPTUNE, Ogre::Vector3 (-150.0f, 150.0f, 500.0f))); 
			planets.push_back(new Planet_Space::Planet(scene_manager, JUPITER, Ogre::Vector3 (100.0f, -75.0f, -320.0f))); 
			break;
		case 1:
			planets.push_back(new Planet_Space::Planet(scene_manager, EARTH, Ogre::Vector3 (-250.0f, -250.0f, 400.0f)));
			planets.push_back(new Planet_Space::Planet(scene_manager, JUPITER, Ogre::Vector3 (150.0f, 300.0f, -200.0f))); 
			break;
		case 2: 
			planets.push_back(new Planet_Space::Planet(scene_manager, NEPTUNE, Ogre::Vector3 (-150.0f, 150.0f, 500.0f))); 
			planets.push_back(new Planet_Space::Planet(scene_manager, EARTH, Ogre::Vector3 (100.0f, -75.0f, -320.0f)));
			planets.push_back(new Planet_Space::Planet(scene_manager, JUPITER, Ogre::Vector3 (0.0f, 500.0f, 0.0f))); 
			break;
		case 3: 
			planets.push_back(new Planet_Space::Planet(scene_manager, VENUS, Ogre::Vector3 (500.0f, -100.0f, 0.0f))); 
			planets.push_back(new Planet_Space::Planet(scene_manager, NEPTUNE, Ogre::Vector3 (-350.0f, 150.0f, -500.0f))); 
			planets.push_back(new Planet_Space::Planet(scene_manager, JUPITER, Ogre::Vector3 (100.0f, -75.0f, -320.0f))); 
			break;
		case 4: 
			planets.push_back(new Planet_Space::Planet(scene_manager, VENUS, Ogre::Vector3 (0.0f, 0.0f, 0.0f))); 
			planets.push_back(new Planet_Space::Planet(scene_manager, NEPTUNE, Ogre::Vector3 (-150.0f, 150.0f, 350.0f))); 
			planets.push_back(new Planet_Space::Planet(scene_manager, EARTH, Ogre::Vector3 (0.0f, 600.0f, 100.0f))); 
			break;
		case 5: 
			planets.push_back(new Planet_Space::Planet(scene_manager, VENUS, Ogre::Vector3 (500.0f, 0.0f, 0.0f))); 
			planets.push_back(new Planet_Space::Planet(scene_manager, EARTH, Ogre::Vector3 (-500.0f, 0.0f, 0.0f))); 
			break;
		case 6: 
			planets.push_back(new Planet_Space::Planet(scene_manager, JUPITER, Ogre::Vector3 (0.0f, 0.0f, 500.0f))); 
			break;
		case 7: 
			planets.push_back(new Planet_Space::Planet(scene_manager, VENUS, Ogre::Vector3 (-150.0f, -750.0f, 200.0f))); 
			planets.push_back(new Planet_Space::Planet(scene_manager, JUPITER, Ogre::Vector3 (0.0f, 0.0f, 0.0f))); 
			planets.push_back(new Planet_Space::Planet(scene_manager, VENUS, Ogre::Vector3 (100.0f, -750.0f, -320.0f))); 
			break;
		case 8: 
			planets.push_back(new Planet_Space::Planet(scene_manager, EARTH, Ogre::Vector3 (0.0f, 400.0f, 0.0f))); 
			planets.push_back(new Planet_Space::Planet(scene_manager, EARTH, Ogre::Vector3 (-250.0f, 50.0f, -250.0f))); 
			planets.push_back(new Planet_Space::Planet(scene_manager, EARTH, Ogre::Vector3 (200.0f, -300.0f, 100.0f))); 
			planets.push_back(new Planet_Space::Planet(scene_manager, EARTH, Ogre::Vector3 (700.0f, 200.0f, -500.0f))); 
			planets.push_back(new Planet_Space::Planet(scene_manager, EARTH, Ogre::Vector3 (0.0f, 0.0f, 0.0f))); 
			break;
		case 9: 
			planets.push_back(new Planet_Space::Planet(scene_manager, VENUS, Ogre::Vector3 (400.0f, 0.0f, 0.0f))); 
			planets.push_back(new Planet_Space::Planet(scene_manager, EARTH, Ogre::Vector3 (0.0f, -300.0f, -250.0f))); 
			planets.push_back(new Planet_Space::Planet(scene_manager, NEPTUNE, Ogre::Vector3 (-150.0f, 150.0f, 500.0f))); 
			planets.push_back(new Planet_Space::Planet(scene_manager, JUPITER, Ogre::Vector3 (100.0f, -700.0f, -320.0f))); 
			break;
		} 

		createAsteroidField(scene_manager);
		createRandomEnemySpray(scene_manager);
	}

	// Creates A Random Enemy Spray For The Level
	void Level::createRandomEnemySpray (Ogre::SceneManager* scene_manager) {
		float nebula_buffer = 100.0f;
		int numOfEnemiesInLevel = 1 + (levelNum * 2);

		for (int i = 0; i < numOfEnemiesInLevel; i++) {
			int enemy_type = rand() % NUM_OF_ENEMY_TYPES + 1;
			float xPos = -nebula_buffer + (((double) rand() / (RAND_MAX)) * (nebula_buffer * 2));
			float yPos = -nebula_buffer + (((double) rand() / (RAND_MAX)) * (nebula_buffer * 2));
			float zPos = -nebula_buffer + (((double) rand() / (RAND_MAX)) * (nebula_buffer * 2));
			enemies.push_back(new Enemy_Space::Enemy (scene_manager, Ogre::Vector3 (xPos, yPos, zPos), enemy_type));
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