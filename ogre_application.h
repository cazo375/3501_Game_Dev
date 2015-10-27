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

/* Thrust Constants */
#define ACCELERATION_STEP 0.3f
#define ORIENTATION_STEP 2.0f
#define MAX_ORIENTATION_THRUST 2.0f
#define MIN_ORIENTATION_THRUST -2.0f	
#define LAZER_THRUST 6.0f
#define LAZER_LIFE_SPAN 12.0f
#define ROTATION_THRUST 1.0f
#define MAX_FORWARD_THRUST 4.0f // Number of elements in the chain
#define MAX_REVRESE_THRUST -4.0f // Number of elements in the chain

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

	/* An asteroid */
	struct Asteroid {
		Ogre::Vector3 pos; // Position
		Ogre::Quaternion ori; // Orientation
		Ogre::Quaternion lm; // Angular momentum (use as velocity)
		Ogre::Vector3 drift; // Drift direction
	};

	struct Lazer {
		Ogre::Vector3 pos;
		Ogre::Vector3 direction;
		Ogre::Quaternion ori;
		float lifeCounter;
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
		void CreateCubeInstance (Ogre::Vector3 pos = Ogre::Vector3 (0.0, 0.0, 0.0));
		void CreateTargetCube (void);
		void MainLoop(void);															// Keep application active

		/* Camera demo */
		void CreateAsteroidField(int num_asteroids); // Create asteroid field
		void CreatePlanetField (void);
		void TransformAsteroidField(void);
		void TransformPlanetField (void);

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

		// Methods For Moving Objects On A Frame Per Frame Basis
		void moveLazer (void);
		void applyQuaternionRotation (Ogre::Camera* c, Ogre::Quaternion &q);

		// Collision Detection Method
		void runCollisionDetection(void);
		void destoryLazer (void);
		void destoryAstroid (int index);


		/*----------------------------------- Variables ------------------------------*/
		std::auto_ptr<Ogre::Root> ogre_root_;

		// Main Ogre window
		Ogre::RenderWindow* ogre_window_;

		/* Animation-related variables */
		bool animating_; // Whether animation is on or off
		bool space_down_; // Whether space key was pressed

		float currentForwardThrust;		// The current thrust of the ship
		float currentSideThrust;		// Curent Side Thruster Amount
		float currentUpDownThrust;		// Current Up Down Thruster
		float currentPitchChange;		// Current Pitch Thruster
		float currentYawChange;			// Current Yaw Thruster
		float currentRollChange;		// Current Roll Thruster

		int num_asteroids_;
		Asteroid asteroid_[MAX_NUM_ASTEROIDS];
		Ogre::SceneNode* cube_[MAX_NUM_ASTEROIDS];
		enum Direction last_dir_;

		// Planets Within The Game
		std::vector<Planet_Space::Planet> planets;

		// Camera Node
		Ogre::SceneNode* camera_node;

		// Input managers
		OIS::InputManager *input_manager_;
		OIS::Mouse *mouse_;
		OIS::Keyboard *keyboard_;

		// Lazer Mechanics
		Lazer currentLazer;
		Ogre::SceneNode* lazer;

		// Target Cube
		Ogre::SceneNode* targetCube;

		// Game Variables
		Level_Manager_Space::Level_Manager level_manager;

	}; // class OgreApplication

} // namespace ogre_application;

#endif // OGRE_APPLICATION_H_
