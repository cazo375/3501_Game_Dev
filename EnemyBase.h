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

#define NEBULA_RADIUS 1500.0f
#define NODE_MIN_RADIUS 1.0f 
#define ENEMY_MOVE_SUPER_SPEED 2.5f
#define ENEMY_MOVE_FAST_SPEED 1.8f
#define ENEMY_MOVE_SLOW_SPEED 0.7f
#define ENEMY_MOVE_SPEED 1.0f
#define PLAYER_HOSTILE_RADIUS 120.0f
#define SAFE_DISTANCE 50.0f
#define NUM_OF_ENEMY_TYPES 3

namespace Enemy_Space {

	enum Ai_State {PROWL, PURSUE, IDLE, FLEE, HALT, INTIMIDATE, CHARGE, RETURN};
	enum enemy_type{PEON, SCOUT, BOSS};


	class Enemy : public Ship_Space::Ship {

	public:
		Enemy (void);
		Enemy (Ogre::SceneManager* scene_manager, Ogre::Vector3 initalPosition, int enemy_num = 1);
		~Enemy (void);

		void fireShot (void);			
		void destoryEnemy (void);			
		void flee(Ogre::Vector3 playerPos);
		void registerHit (int damageAmount);
		void prowl(Ogre::Vector3 playerPos);
		void pursue(Ogre::Vector3 playerPos);
		void RotateShip(Ogre::Vector3 target);
		void spotPlayer(Ogre::Vector3 playerPos);
		void returnToBattle(void);
		void charge(Ogre::Vector3 playerPos);
		void intimidate(Ogre::Vector3 playerPos);
		void shouldFireShot (Player_Space::Player*);
		void advance (Player_Space::Player* player, Ogre::Real);				

		// Getters And Setters
		int getEnemyHealth(void);
		float getBoundingCircleRadius(void);
		Ogre::Vector3 getPosition(void);
		Ogre::Vector3 getDirection(void);
		boolean isInArea();
		boolean inHostileRange(Ogre::Vector3 playerPos);
		boolean inSpotRange(Ogre::Vector3 playerPos);
		boolean inIntimidateRange(Ogre::Vector3 playerPos);
		boolean enemyDead (void);
		boolean should_run_collision (void);

	protected:
		Ogre::SceneNode* enemy_node;
		Ogre::SceneManager* scene_manager;
		Ogre::Vector3 original_position;
		Ogre::Vector3 currentDirection;
		Ogre::String entity_name;
		boolean alive;

		// Creation Functions
		float GetMagnatude(Ogre::Vector3);
		Ogre::Vector3 toOrigin(void);
		Ogre::Matrix4 RotationMatrix(Ogre::Vector3 axis, Ogre::Radian angle);
		Ogre::Matrix4 TranslationMatrix(Ogre::Vector3 trans);
		Ogre::Matrix4 ScalingMatrix(Ogre::Vector3 scale);
		Ogre::Vector3 GetVectorFromTwoPoints(Ogre::Vector3 playerpos, Ogre::Vector3 Enemypos);
		Ogre::Vector3 RandomVector3(void);
		void AssignTransf(Ogre::SceneNode* node, Ogre::Matrix4 transf);
		void UpdateTransf(Ogre::SceneNode* node, Ogre::Matrix4 transf);

		void createEnemyByNum (Ogre::SceneManager*, Ogre::Vector3, int num = 4);	
		void CreateEnemy1 (Ogre::SceneManager*, Ogre::Vector3);
		void CreateEnemy2 (Ogre::SceneManager*, Ogre::Vector3);
		void CreateEnemy3 (Ogre::SceneManager*, Ogre::Vector3);
		void CreateEnemy4 (Ogre::SceneManager*, Ogre::Vector3);
		void CreateEnemy5 (Ogre::SceneManager*, Ogre::Vector3);


		// Path Related Variables
		std::vector<Ogre::Vector3> pathPoints;
		void buildPointGraph(void);
		void cycleGraphPointIfNeeded(void);
		int currentPathIndex;
		Ogre::Vector3 initialDir;


		int STATE;
		int ENEMY_TYPE;
		Ogre::Real wait;
		Ogre::Real lifeSpan;
		Ogre::Real sleep;

		// Collision Related Variables
		float boundingSphereRadius;
	};
}