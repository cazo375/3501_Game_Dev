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
#include "Ship.h"
#include <vector>
#include "Player.h"

#define NODE_MIN_RADIUS 1.0f 
#define ENEMY_MOVE_SPEED 0.10f
#define PLAYER_HOSTILE_RADIUS 50.0f

namespace Enemy_Space {

	class Enemy : public Ship_Space::Ship {
	public:
		Enemy (void);
		Enemy (Ogre::SceneManager* scene_manager, Ogre::Vector3 initalPosition);
		~Enemy (void);

		void advance (void);							// Called On A Per Frame Basis... Causes The Enemy To Advance To It's Next Frame
		void destoryEnemy (void);						// Destroys Our Enemy And Removes Them From The Game
		void registerHit (int damageAmount);			// Registers The Hit On The Enemy
		void fireShot (void);
		void shouldFireShot (Player_Space::Player*);

		// Getters And Setters
		int getEnemyHealth(void);
		float getBoundingCircleRadius(void);
		boolean enemyDead (void);

	protected:
		Ogre::SceneNode* enemy_node;
		Ogre::SceneManager* scene_manager;
		Ogre::Vector3 original_position;
		Ogre::Vector3 currentDirection;
		Ogre::String entity_name;
		boolean alive;

		// Creation Functions
		void createEnemy (Ogre::SceneManager*, Ogre::Vector3);

		// Path Related Variables
		std::vector<Ogre::Vector3> pathPoints;
		void buildPointGraph(void);
		void cycleGraphPointIfNeeded(void);
		int currentPathIndex;

		// Collision Related Variables
		int enemyHealth;
		float boundingSphereRadius;
	};
}