// Collision Manager For Our Small Game
#include <vector>
#include "OGRE/OgreRoot.h"
#include "OGRE/OgreRenderSystem.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreWindowEventUtilities.h"
#include "OGRE/OgreManualObject.h"
#include "OGRE/OgreEntity.h"
#include "OGRE/Overlay/OgreFontManager.h"
#include "OGRE/Overlay/OgreTextAreaOverlayElement.h"
#include "OGRE/Overlay/OgreOverlayManager.h"
#include "OGRE/Overlay/OgreOverlaySystem.h"
#include "OIS/OIS.h"
#include "WeaponShot.h"
#include "Weapon.h"
#include "Ship.h"

#define ACCELERATION_STEP 0.15f
#define ORIENTATION_STEP 2.0f
#define MAX_ORIENTATION_THRUST 2.0f
#define MIN_ORIENTATION_THRUST -2.0f	
#define LAZER_THRUST 6.0f
#define WEAPON_SWITCH_DELAY 0.25f
#define LAZER_LIFE_SPAN 12.0f
#define ROTATION_THRUST 1.0f
#define MAX_FORWARD_THRUST 1.5f // Number of elements in the chain
#define MAX_REVRESE_THRUST -1.5f // Number of elements in the chain
#define COLLISION_DETECTION_RAD 3.0f
#define PLAYER_STARTING_HEALTH 50

#define PLAYER_ENEMY_STRING "Remaining Enemies: "
#define PLAYER_WEAPON_STRING "Current Weapon: "
#define PLAYER_HEALTH_STRING "Current Health: "

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
		void advance(Ogre::Real);
		void fireShot(void);
		void cycle_shot(void);
		void registerHit(int damageAmount);

		// Collision Detection Variables
		void resetPosition(void);
		float getBoundingCircleRadius(void);

		// Methods For Updating The Player
		void updatePlayerUI (int enemies_remaining, int level_num);

		// Getters
		Ogre::Vector3 getDirection();

	private:
		// Variables
		Ogre::SceneManager* scene_manager;
		Ogre::Camera* player_camera;
		Ogre::SceneNode* targetCube;
		Ogre::Vector3 initialPosition;
		Ogre::Quaternion initialOrientation;

		// Texts
		Ogre::TextAreaOverlayElement* current_weapon_text;
		Ogre::TextAreaOverlayElement* amount_of_enemies_text;
		Ogre::TextAreaOverlayElement* current_player_health_text;
		Ogre::TextAreaOverlayElement* current_level_text;

		// Movement Variables		
		float currentForwardThrust;		// The current thrust of the ship
		float currentSideThrust;		// Curent Side Thruster Amount
		float currentUpDownThrust;		// Current Up Down Thruster
		float currentPitchChange;		// Current Pitch Thruster
		float currentYawChange;			// Current Yaw Thruster
		float currentRollChange;		// Current Roll Thruster

		// Weapon Variables
		float weaponTimer;

		// Player Variables
		int player_health;

		// Functions
		void applyRotation (Ogre::Quaternion& quat);
		void destroyFiredWeapon (void);
		void initialize(void);
		void initOverlay(void);
		void initTextArea (Ogre::TextAreaOverlayElement* area);
	};
}

#endif