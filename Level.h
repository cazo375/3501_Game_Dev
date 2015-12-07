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
#include "Asteroid.h"

#ifndef LEVEL_H
#define LEVEL_H

#define NUM_ASTEROIDS_PER_LEVEL 240

// The Actual Space For Levels... They Will Be Cycled Through The Level_Manager
namespace Level_Space {
	class Level
	{
	public:
		Level(void);
		Level(int levelNum);
		~Level(void);

		void advance (Player_Space::Player* player, Ogre::Real);
		void createLevel (Ogre::SceneManager* scene_manager);
		void destoryLevel (Ogre::SceneManager* scene_manager);
		void createRandomEnemySpray (Ogre::SceneManager* scene_manager);

		void destoryEnemyAt (int index);
		void destoryAsteroidAt (int index);

		std::vector<Planet_Space::Planet*> getPlanets(void);
		std::vector<Enemy_Space::Enemy*> getEnemies (void);
		std::vector<Asteroid_Space::Asteroid*> getAsteroids (void);
	private:
		int levelNum;
		std::vector<Enemy_Space::Enemy*> enemies;
		std::vector<Planet_Space::Planet*> planets;
		std::vector<Asteroid_Space::Asteroid*> asteroids;

		void createAsteroidField (Ogre::SceneManager*);
	};
}

#endif