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
#include "Ship.h"

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

#ifndef PLAYER_H
#define PLAYER_H

namespace Player_Space {

	class Player: public Ship_Space::Ship {
	public:
		Player(Ogre::SceneManager* manager, Ogre::Camera* camera);
		~Player(void);

		// Movement Methods
		void applyKeyEvent (OIS::Keyboard* keyboard);
		void createPlayer(void);
		void advance(void);
		void fireShot(void);

		// Collision Detection Variables
		void resetPosition(void);
		float getBoundingCircleRadius(void);

	private:
		// Variables
		Ogre::SceneManager* scene_manager;
		Ogre::Camera* player_camera;
		Ogre::SceneNode* targetCube;
		Ogre::Vector3 initialPosition;
		Ogre::Quaternion initialOrientation;

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
		void initialize(void);
	};
}

#endif