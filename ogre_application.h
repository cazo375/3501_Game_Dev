#ifndef OGRE_APPLICATION_H_
#define OGRE_APPLICATION_H_

#include <exception>
#include <string>
#include <vector>

#include "OGRE/OgreRoot.h"
#include "OGRE/OgreRenderSystem.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreWindowEventUtilities.h"
#include "OGRE/OgreManualObject.h"
#include "OGRE/OgreEntity.h"
#include "OIS/OIS.h"

#include "CollisionDetector.h"
#include "MeshManager.h"
#include "Level_Manager.h"
#include "Player.h"
#include "Explosion.h"

/* Thrust Constants */
#define ACCELERATION_STEP 0.3f
#define ORIENTATION_STEP 2.0f
#define MAX_ORIENTATION_THRUST 2.0f
#define MIN_ORIENTATION_THRUST -2.0f	
#define LAZER_THRUST 6.0f
#define LAZER_LIFE_SPAN 12.0f
#define ROTATION_THRUST 1.0f

/* Camera demo variables */
#define MAX_NUM_ASTEROIDS 500 // Number of elements in the chain

namespace ogre_application {


	/* A useful type to define */
	typedef std::vector<Ogre::String> Strings;

	/* Our exception type */
	class OgreAppException: public std::exception
	{
	private:
		std::string message_;
	public:
		OgreAppException(std::string message) : message_(message) {};
		virtual const char* what() const throw() { return message_.c_str(); };
	};

	/* Possible directions of the ship */
	enum Direction { Forward, Backward, Up, Down, Left, Right };

	/* Our Ogre application */
	class OgreApplication :
		public Ogre::FrameListener, // Derive from FrameListener to be able to have render event callbacks
		public Ogre::WindowEventListener // Derive from FrameListener to be able to have window event callbacks
	{

	public:
		OgreApplication(void);
		void Init(void);																// Call Init() before running the main loop
		void MainLoop(void);			// Keep application active

		Ogre::Matrix4 RotationMatrix(Ogre::Vector3 axis, Ogre::Radian angle);
		Ogre::Matrix4 TranslationMatrix(Ogre::Vector3 trans);
		Ogre::Matrix4 ScalingMatrix(Ogre::Vector3 scale);
		Ogre::Vector3 getVectorFromTwoPoints(Ogre::Vector3 playerpos, Ogre::Vector3 Enemypos);
		void AssignTransf(Ogre::SceneNode* node, Ogre::Matrix4 transf);


		/* Camera demo */
		void CreatePlanetField (void);
		void TransformPlanetField (void);
		void CreateEnemy1 (void);
		void CreateEnemy2 (void);
		void CreateEnemy3 (void);

		/* Level Methods*/
		void startGame(void);

	private:

		/*----------------------------------- Functions ------------------------------*/
		/* Methods to initialize the application */
		void InitRootNode(void);
		void InitPlugins(void);
		void InitRenderSystem(void);
		void InitWindow(void);
		void InitViewport(void);
		void InitEvents(void);
		void InitOIS(void);
		void LoadMaterials(void);

		/* Methods to handle events */
		bool frameRenderingQueued(const Ogre::FrameEvent& fe);
		void windowResized(Ogre::RenderWindow* rw);

		// Collision Detection Method
		void runCollisionDetection(void);
		void runLazerCollisionDetection (void);
		void runPlanetCollisionDetection (void);
		void destoryAstroid (int index);

		// Enemies Shoot A Players
		void haveEnemiesShootAtPlayer (void);

		// Explosion Stuff
		std::vector<Explosion_Space::Explosion*> active_explosions;
		void spawnExplosionAt (Ogre::Vector3);
		void deleteAllExpiredExplosions (void);
		void advanceAllExplosions (Ogre::Real);


		/*----------------------------------- Variables ------------------------------*/
		std::auto_ptr<Ogre::Root> ogre_root_;

		// Main Ogre window
		Ogre::RenderWindow* ogre_window_;

		/* Animation-related variables */
		bool animating_; // Whether animation is on or off
		bool space_down_; // Whether space key was pressed

		
		// Movement Variables		
		float currentForwardThrust;		// The current thrust of the ship
		float currentSideThrust;		// Curent Side Thruster Amount
		float currentUpDownThrust;		// Current Up Down Thruster
		float currentPitchChange;		// Current Pitch Thruster
		float currentYawChange;			// Current Yaw Thruster
		float currentRollChange;		// Current Roll Thruster

		Ogre::SceneNode* enemy1[1];
		Ogre::SceneNode* enemy2[7];
		Ogre::SceneNode* enemy3[7];


		// Planets Within The Game
		Planet_Space::Planet nebula;

		// Camera Node
		Ogre::SceneNode* camera_node;

		// Input managers
		OIS::InputManager *input_manager_;
		OIS::Mouse *mouse_;
		OIS::Keyboard *keyboard_;

		Player_Space::Player* player;

		// Target Cube
		Ogre::SceneNode* targetCube;

		// Game Variables
		Level_Manager_Space::Level_Manager level_manager;

	}; // class OgreApplication

} // namespace ogre_application;

#endif // OGRE_APPLICATION_H_
