// Collision Manager For Our Small Game
#include <vector>
#include "OGRE/OgreRoot.h"
#include "OGRE/OgreRenderSystem.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreWindowEventUtilities.h"
#include "OGRE/OgreManualObject.h"
#include "OGRE/OgreEntity.h"
#include "OIS/OIS.h"
#include "WeaponBase.h"

#define ACCELERATION_STEP 0.3f
#define ORIENTATION_STEP 2.0f
#define MAX_ORIENTATION_THRUST 2.0f
#define MIN_ORIENTATION_THRUST -2.0f	
#define LAZER_THRUST 6.0f
#define LAZER_LIFE_SPAN 12.0f
#define ROTATION_THRUST 1.0f
#define MAX_FORWARD_THRUST 4.0f // Number of elements in the chain
#define MAX_REVRESE_THRUST -4.0f // Number of elements in the chain
#define COLLISION_DETECTION_RAD 1.0f

namespace Player_Space {

	struct Lazer {
		Ogre::Vector3 pos;
		Ogre::Vector3 direction;
		Ogre::Quaternion ori;
		float lifeCounter;
	};

	class Player {
	public:
		Player(Ogre::SceneManager* manager, Ogre::Camera* camera, Ogre::Vector3 pos = Ogre::Vector3(0.0f, 0.0f, 0.0f));
		~Player(void);

		// Movement Methods
		void applyKeyEvent (OIS::Keyboard* keyboard);
		void createPlayer(void);
		void advance(void);
		void fireWeapon (void);

		Weapon_Space::Weapon_Shot* getCurrentLazer(void);

		// Collision Detection Variables
		void resetPosition(void);
		float getBoundingCircleRadius(void);
		Ogre::Vector3 getPlayerPosition (void);

	private:
		// Variables
		Ogre::SceneManager* scene_manager;
		Ogre::Camera* player_camera;
		Ogre::SceneNode* camera_node;
		Ogre::SceneNode* targetCube;
		Ogre::Vector3 initialPosition;
		Ogre::Quaternion initialOrientation;

		// Lazer Mechanics
		Weapon_Space::Weapon_Shot* test_shot;

		// Movement Variables		
		float currentForwardThrust;		// The current thrust of the ship
		float currentSideThrust;		// Curent Side Thruster Amount
		float currentUpDownThrust;		// Current Up Down Thruster
		float currentPitchChange;		// Current Pitch Thruster
		float currentYawChange;			// Current Yaw Thruster
		float currentRollChange;		// Current Roll Thruster

		// Functions
		void applyRotation (Ogre::Quaternion& quat);
		void destroyFiredWeapon (void);
		void moveLazer (void);
		void initialize(void);
	};
}