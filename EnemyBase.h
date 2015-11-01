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
#include <vector>

#define NODE_MIN_RADIUS 1.0f 
#define ENEMY_MOVE_SPEED 0.10f

namespace Enemy_Space {

	class Enemy {
	public:
		Enemy (void);
		Enemy (Ogre::SceneManager* scene_manager, Ogre::Vector3 initalPosition);
		~Enemy (void);

		void advance (void);		// Called On A Per Frame Basis... Causes The Enemy To Advance To It's Next Frame
		void destoryEnemy (void);	

		// Getters And Setters
		int getEnemyHealth(void);
		float getBoundingSphereRadius(void);
		Ogre::Vector3 getPosition(void);

	protected:
		Ogre::SceneNode* enemy_node;
		Ogre::SceneManager* scene_manager;
		Ogre::Vector3 original_position;

		Ogre::String entity_name;
		boolean alive;

		// Path Related Variables
		std::vector<Ogre::Vector3> pathPoints;
		void buildPointGraph(void) ;
		int currentPathIndex;

		// Collision Related Variables
		int enemyHealth;
		float boundingSphereRadius;
		
	};
}