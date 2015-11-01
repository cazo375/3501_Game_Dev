#include "Player.h"

namespace Player_Space {		

	// Player Constructor
	Player::Player(Ogre::SceneManager* manager, Ogre::Camera* camera, Ogre::Vector3 pos) {
		scene_manager = manager;
		player_camera = camera;
		initialPosition = camera->getPosition();
		initialOrientation = camera->getOrientation();
		test_shot = nullptr;
		initialize();
	}

	Player::~Player() {
	}

	// Advances the player by a frame
	void Player::advance (void) {
		player_camera->setPosition(player_camera->getPosition() + player_camera->getDirection()*currentForwardThrust);
		player_camera->setPosition(player_camera->getPosition() + player_camera->getUp()*currentUpDownThrust);
		player_camera->setPosition(player_camera->getPosition() + player_camera->getRight()*currentSideThrust);
		camera_node->setPosition(player_camera->getPosition());

		moveLazer();
	}

	// Apply Our Quaterion Rotations To What We Need
	void Player::applyRotation(Ogre::Quaternion& q) {
		player_camera->rotate(q);
		camera_node->setOrientation(player_camera->getOrientation());
	}

	// Applys A Captured Key Event To This Player
	void Player::applyKeyEvent (OIS::Keyboard* keyboard_) {
		if (keyboard_->isKeyDown (OIS::KC_G)) {
			initialize();
		}

		/* Camera translation */
		if (keyboard_->isKeyDown(OIS::KC_W)){
			currentForwardThrust = std::min (currentForwardThrust + ACCELERATION_STEP, MAX_FORWARD_THRUST);
		}

		if (keyboard_->isKeyDown(OIS::KC_S)){
			currentForwardThrust = std::max (currentForwardThrust - ACCELERATION_STEP, MAX_REVRESE_THRUST);
		}

		if (keyboard_->isKeyDown(OIS::KC_Q)){
			currentUpDownThrust = std::max (currentUpDownThrust - ACCELERATION_STEP, MAX_REVRESE_THRUST);
		}

		if (keyboard_->isKeyDown(OIS::KC_E)){
			currentUpDownThrust = std::min (currentUpDownThrust + ACCELERATION_STEP, MAX_FORWARD_THRUST);
		}

		if (keyboard_->isKeyDown(OIS::KC_D)){
			currentSideThrust = std::min (currentSideThrust + ACCELERATION_STEP, MAX_FORWARD_THRUST);
		}

		if (keyboard_->isKeyDown(OIS::KC_A)){
			currentSideThrust = std::max (currentSideThrust - ACCELERATION_STEP, MAX_REVRESE_THRUST);
		}

		if (keyboard_->isKeyDown(OIS::KC_UP)){
			applyRotation(Ogre::Quaternion(Ogre::Degree(ROTATION_THRUST), player_camera->getRight()));
		}

		if (keyboard_->isKeyDown(OIS::KC_DOWN)){
			applyRotation(Ogre::Quaternion(Ogre::Degree(-ROTATION_THRUST), player_camera->getRight()));
		}

		if (keyboard_->isKeyDown(OIS::KC_LEFT)){
			applyRotation(Ogre::Quaternion(Ogre::Degree(ROTATION_THRUST), player_camera->getUp()));
		}

		if (keyboard_->isKeyDown(OIS::KC_RIGHT)){
			applyRotation(Ogre::Quaternion(Ogre::Degree(-ROTATION_THRUST), player_camera->getUp()));
		}

		if (keyboard_->isKeyDown(OIS::KC_Z)){
			applyRotation(Ogre::Quaternion(Ogre::Degree(-ROTATION_THRUST), player_camera->getDirection()));
		}

		if (keyboard_->isKeyDown(OIS::KC_X)){
			applyRotation(Ogre::Quaternion(Ogre::Degree(ROTATION_THRUST), player_camera->getDirection()));
		}

		if (keyboard_->isKeyDown (OIS::KC_F)) {
			fireWeapon();
		}

	}

	void Player::initialize (void) {
		currentForwardThrust = 0;
		currentSideThrust = 0;
		currentUpDownThrust = 0;	
		currentPitchChange = 0;	
		currentYawChange = 0;	
		currentRollChange = 0;	
	}

	// Creates Our Player For Our Came
	void Player::createPlayer(void) {
		Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();
		Ogre::Entity *entity = scene_manager->createEntity("player_target.cube", "cube.mesh");

		camera_node = root_scene_node->createChildSceneNode();
		camera_node->setPosition(player_camera->getPosition());
		targetCube = camera_node->createChildSceneNode("player_target.cube");
		targetCube->attachObject(entity);
		targetCube->translate(0.0f, 0.0f, -30.0f);
	}

	// Fires The Weapon If Hasn't Been Fired
	void Player::fireWeapon (void) {
		if (!test_shot) {
			test_shot = new Weapon_Space::Weapon_Shot (scene_manager, camera_node->getPosition(), (targetCube->_getDerivedPosition() - camera_node->getPosition()).normalisedCopy());
		}
	}

	// Moves Our Lazer If It Is Defined
	void Player::moveLazer(void) {
		if (test_shot) {
			test_shot->moveShot();

			if (test_shot->shouldDestoryShot()) {
				delete test_shot;
				test_shot = nullptr;
			}
		}
	}

	Weapon_Space::Weapon_Shot* Player::getCurrentLazer(void) {
		return test_shot;
	}

	float Player::getBoundingCircleRadius (void) {
		return COLLISION_DETECTION_RAD;
	}

	Ogre::Vector3 Player::getPlayerPosition(void) {
		return camera_node->getPosition();
	}

	void Player::resetPosition(void) {
		camera_node->setPosition(initialPosition);
		camera_node->setOrientation(initialOrientation);

		player_camera->setPosition(initialPosition);
		player_camera->setOrientation(initialOrientation);
	}
}