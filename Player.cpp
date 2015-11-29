#include "Player.h"

namespace Player_Space {		

	// Player Constructor
	Player::Player(Ogre::SceneManager* manager, Ogre::Camera* camera) {
		scene_manager = manager;
		player_camera = camera;
		initialPosition = camera->getPosition();
		initialOrientation = camera->getOrientation();
		shot = nullptr;
		text_area = nullptr;
		weaponTimer = 0;
		health = 50;
		currentWeaponIndex = 0;

		// Weapon Bank
		weapons.push_back(new Weapon_Space::Lazer("player"));
		weapons.push_back(new Weapon_Space::Bomb("player"));
		weapons.push_back(new Weapon_Space::Scatter_Shot("player"));
		weapons.push_back(new Weapon_Space::Scatter_Bomb_Shot("player"));

		initialize();
		initOverlay();
	}

	Player::~Player() {
	}

	// Advances the player by a frame
	void Player::advance (Ogre::Real time) {
		player_camera->setPosition(player_camera->getPosition() + player_camera->getDirection()*currentForwardThrust);
		player_camera->setPosition(player_camera->getPosition() + player_camera->getUp()*currentUpDownThrust);
		player_camera->setPosition(player_camera->getPosition() + player_camera->getRight()*currentSideThrust);
		ship_node->setPosition(player_camera->getPosition());

		weapons[currentWeaponIndex]->advance(time);

		if (weaponTimer < WEAPON_SWITCH_DELAY) {
			weaponTimer += time;
		}

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

		if (keyboard_->isKeyDown (OIS::KC_C)) {
			cycle_shot();
		}

	}

	// Cycles The Player's Gun When Called
	void Player::cycle_shot (void) {
		if (weaponTimer >= WEAPON_SWITCH_DELAY) {
			weaponTimer = 0;

			weapons[currentWeaponIndex]->removeAllShots();
			currentWeaponIndex++;

			if (currentWeaponIndex >= weapons.size()) {
				currentWeaponIndex = 0;
			}

			text_area->setCaption("Current Weapon: " + weapons[currentWeaponIndex]->getWeaponName());
			std::cout << "Current Weapon: " << weapons[currentWeaponIndex]->getWeaponName() << std::endl;
		}
	}

	void Player::initialize (void) {
		currentForwardThrust = 0;
		currentSideThrust = 0;
		currentUpDownThrust = 0;	
		currentPitchChange = 0;	
		currentYawChange = 0;	
		currentRollChange = 0;	
		currentWeaponIndex = 0;
	}

	// Creates Our Player For Our Came
	void Player::createPlayer(void) {
		Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();
		Ogre::Entity *entity = scene_manager->createEntity("player_target.cube", "cube.mesh");
		entity->setMaterialName("CrosshairMaterial");

		ship_node = root_scene_node->createChildSceneNode();
		ship_node->setPosition(player_camera->getPosition());

		targetCube = ship_node->createChildSceneNode("player_target.cube");
		targetCube->attachObject(entity);
		targetCube->translate(0.0f, 0.0f, -30.0f);
	}

	// Fires The Weapon If Hasn't Been Fired
	void Player::fireShot (void) {
		if (weapons[currentWeaponIndex]->getOrientationNeeded()) {
			weapons[currentWeaponIndex]->fire_weapon(scene_manager, ship_node->getPosition(), 
				(targetCube->_getDerivedPosition() - ship_node->getPosition()).normalisedCopy(), ship_node->getOrientation());
		}
		else {
			weapons[currentWeaponIndex]->fire_weapon(scene_manager, ship_node->getPosition(), (targetCube->_getDerivedPosition() - ship_node->getPosition()).normalisedCopy());
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

		initialize();
	}

	void Player::initOverlay(void) {
		// Create and initialize the overlay system
		Ogre::OverlaySystem *os = new Ogre::OverlaySystem();
		scene_manager->addRenderQueueListener(os);

		// Initialize a font: assumes a standard Windows system
		Ogre::ResourceGroupManager& resource_group_manager = Ogre::ResourceGroupManager::getSingleton();
		resource_group_manager.addResourceLocation("C:\\Windows\\Fonts", "FileSystem");
		Ogre::FontManager& font_manager = Ogre::FontManager::getSingleton();
		Ogre::ResourcePtr font = font_manager.create("MyFont", "General");
		font->setParameter("type", "truetype");
		font->setParameter("source", "arial.ttf");
		font->setParameter("size", "26");
		font->setParameter("resolution", "96");
		font->load();

		// Create a panel for the overlay
		Ogre::OverlayManager& overlay_manager = Ogre::OverlayManager::getSingleton();
		Ogre::OverlayContainer* panel = (Ogre::OverlayContainer*) overlay_manager.createOverlayElement("Panel", "MyPanel");
		panel->setMetricsMode(Ogre::GMM_PIXELS);
		panel->setPosition(0, 0);
		panel->setDimensions(200, 100);

		// Create a text area and add it to the panel
		text_area = static_cast<Ogre::TextAreaOverlayElement*>(overlay_manager.createOverlayElement("TextArea", "MyTextArea"));
		text_area->setMetricsMode(Ogre::GMM_PIXELS);
		text_area->setPosition(0, 0);
		text_area->setDimensions(200, 100);
		text_area->setFontName("MyFont");
		text_area->setCaption("Current Weapon: " + weapons[currentWeaponIndex]->getWeaponName());
		text_area->setCharHeight(26);
		text_area->setColour(Ogre::ColourValue(0.8, 0.0, 0.0));
		panel->addChild(text_area);

		// Create an overlay using the panel
		Ogre::Overlay* overlay = overlay_manager.create("MyOverlay");
		overlay->add2D(panel);
		overlay->show();
	}

	// Registers A Hit On The Player
	void Player::registerHit (int damageAmount) {
		health = std::max (health - damageAmount, 0);
		std::cout << "Player Has Been Hit... Health Now: " << health << std::endl;
	}
}