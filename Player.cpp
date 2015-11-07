#include "Player.h"

namespace Player_Space {		

	// Player Constructor
	Player::Player(Ogre::SceneManager* manager, Ogre::Camera* camera, Ogre::Vector3 pos) {
		scene_manager = manager;
		player_camera = camera;
		initialPosition = camera->getPosition();
		initialOrientation = camera->getOrientation();
		shot = nullptr;
		initialize();
	}

	Player::~Player() {
	}

	// Advances the player by a frame
	void Player::advance (void) {
		player_camera->setPosition(player_camera->getPosition() + player_camera->getDirection()*currentForwardThrust);
		player_camera->setPosition(player_camera->getPosition() + player_camera->getUp()*currentUpDownThrust);
		player_camera->setPosition(player_camera->getPosition() + player_camera->getRight()*currentSideThrust);
		ship_node->setPosition(player_camera->getPosition());

		moveLazer();
	}

	// Apply Our Quaterion Rotations To What We Need
	void Player::applyRotation(Ogre::Quaternion& q) {
		player_camera->rotate(q);
		ship_node->setOrientation(player_camera->getOrientation());
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
			fireShot();
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

		ship_node = root_scene_node->createChildSceneNode();
		ship_node->setPosition(player_camera->getPosition());
		targetCube = ship_node->createChildSceneNode("player_target.cube");
		targetCube->attachObject(entity);
		targetCube->translate(0.0f, 0.0f, -30.0f);
	}

	// Fires The Weapon If Hasn't Been Fired
	void Player::fireShot (void) {
		if (!shot) {
			shot = new Weapon_Space::Weapon_Shot (scene_manager, ship_node->getPosition(), (targetCube->_getDerivedPosition() - ship_node->getPosition()).normalisedCopy(), "player.shot");
		}
	}


	float Player::getBoundingCircleRadius (void) {
		return COLLISION_DETECTION_RAD;
	}

	void Player::resetPosition(void) {
		ship_node->setPosition(initialPosition);
		ship_node->setOrientation(initialOrientation);

		player_camera->setPosition(initialPosition);
		player_camera->setOrientation(initialOrientation);
	}
}