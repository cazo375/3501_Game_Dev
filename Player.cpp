#include "Player.h"

namespace Player_Space {		

	// Player Constructor
	Player::Player(Ogre::SceneManager* manager, Ogre::Viewport* port, std::vector<Ogre::Camera*> cameras) {
		scene_manager = manager;
		viewport = port;
		player_cameras = cameras;
		weaponTimer = 0;
		weaponPositionToFire = 0;
		health = PLAYER_STARTING_HEALTH;
		currentWeaponIndex = 0;
		cameraToDisplay = -1;
		current_camera_switch_delay = CAMERA_SWITCH_DELAY;

		// Pointers
		shot = nullptr;
		current_weapon_text = nullptr;
		amount_of_enemies_text = nullptr;
		current_player_health_text = nullptr;
		ship_node = nullptr;
		camera_node = nullptr;

		// Setup our cameras
		switchToNextCamera();
		initialPosition = player_cameras[cameraToDisplay]->getPosition();
		initialOrientation = player_cameras[cameraToDisplay]->getOrientation();

		// Weapon Bank
		weapons.push_back(new Weapon_Space::Lazer("player"));
		weapons.push_back(new Weapon_Space::Bomb("player"));
		weapons.push_back(new Weapon_Space::Scatter_Shot("player"));
		weapons.push_back(new Weapon_Space::Scatter_Bomb_Shot("player"));
		weapons.push_back(new Weapon_Space::Spline_Bomb_Launcher ("player"));

		initialize();
		initOverlay();
	}

	Player::~Player() {
	}

	// Switches To The Next Camera When Called
	void Player::switchToNextCamera(void) {
		if (current_camera_switch_delay >= CAMERA_SWITCH_DELAY) {
			if (++cameraToDisplay >= player_cameras.size()) {
				cameraToDisplay = 0;
			}
			current_camera_switch_delay = 0;
			viewport->setCamera(player_cameras[cameraToDisplay]);

			if (camera_node && ship_node) {
				// Now We Need To Setup Our Ship
				Ogre::Vector3 variance_from_camera;
				if (cameraToDisplay == 0) {
					variance_from_camera = Ogre::Vector3(0.0, 0.0, 0.0);
				} else {
					variance_from_camera =  player_cameras[0]->getPosition() - player_cameras[cameraToDisplay]->getPosition();
				}

				// Set our Position Properly
				camera_node->setPosition(player_cameras[cameraToDisplay]->getPosition());
				ship_node->setPosition(Ogre::Vector3(0.0, 0.0, 0.0));
				ship_node->translate(variance_from_camera);
			}
		}
	}

	// Advances the player by a frame
	void Player::advance (Ogre::Real time) {
		camera_node->translate(player_cameras[cameraToDisplay]->getDirection()*currentForwardThrust);
		camera_node->translate(player_cameras[cameraToDisplay]->getUp()*currentUpDownThrust);
		camera_node->translate(player_cameras[cameraToDisplay]->getRight()*currentSideThrust);

		for (int i = 0; i < player_cameras.size(); i++) {
			player_cameras[i]->setPosition(player_cameras[i]->getPosition() + player_cameras[cameraToDisplay]->getDirection()*currentForwardThrust);
			player_cameras[i]->setPosition(player_cameras[i]->getPosition() + player_cameras[cameraToDisplay]->getUp()*currentUpDownThrust);
			player_cameras[i]->setPosition(player_cameras[i]->getPosition() + player_cameras[cameraToDisplay]->getRight()*currentSideThrust);
		}

		weapons[currentWeaponIndex]->advance(time);

		if (weaponTimer < WEAPON_SWITCH_DELAY) {
			weaponTimer += time;
		}

		if (current_camera_switch_delay < CAMERA_SWITCH_DELAY) {
			current_camera_switch_delay += time;
		}

		moveLazer();

	}

	// Apply Our Quaterion Rotations To What We Need
	void Player::applyRotation(Ogre::Quaternion& q) {
		for (int i = 0; i < player_cameras.size(); i++) {
			player_cameras[i]->rotate(q);
		}
		camera_node->setOrientation(player_cameras[cameraToDisplay]->getOrientation());
	}

	// Applys A Captured Key Event To This Player
	void Player::applyKeyEvent (OIS::Keyboard* keyboard_) {
		if (keyboard_->isKeyDown (OIS::KC_G)) {
			initialize();
		}
		if (keyboard_->isKeyDown(OIS::KC_P)){
			switchToNextCamera();
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
			applyRotation(Ogre::Quaternion(Ogre::Degree(ROTATION_THRUST), player_cameras[cameraToDisplay]->getRight()));
		}

		if (keyboard_->isKeyDown(OIS::KC_DOWN)){
			applyRotation(Ogre::Quaternion(Ogre::Degree(-ROTATION_THRUST), player_cameras[cameraToDisplay]->getRight()));
		}

		if (keyboard_->isKeyDown(OIS::KC_LEFT)){
			applyRotation(Ogre::Quaternion(Ogre::Degree(ROTATION_THRUST), player_cameras[cameraToDisplay]->getUp()));
		}

		if (keyboard_->isKeyDown(OIS::KC_RIGHT)){
			applyRotation(Ogre::Quaternion(Ogre::Degree(-ROTATION_THRUST), player_cameras[cameraToDisplay]->getUp()));
		}

		if (keyboard_->isKeyDown(OIS::KC_Z)){
			applyRotation(Ogre::Quaternion(Ogre::Degree(-ROTATION_THRUST), player_cameras[cameraToDisplay]->getDirection()));
		}

		if (keyboard_->isKeyDown(OIS::KC_X)){
			applyRotation(Ogre::Quaternion(Ogre::Degree(ROTATION_THRUST), player_cameras[cameraToDisplay]->getDirection()));
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

			current_weapon_text->setCaption("Current Weapon: " + weapons[currentWeaponIndex]->getWeaponName());
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
		health = PLAYER_STARTING_HEALTH;

		if (current_player_health_text) {
			current_player_health_text->setCaption("Current Health: " + Ogre::StringConverter::toString(health) + "/" + Ogre::StringConverter::toString(PLAYER_STARTING_HEALTH));
		}
	}

	// Creates Our Player For Our Came
	void Player::createPlayer(void) {
		Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();
		Ogre::Entity *entity = scene_manager->createEntity("player_target.cube", "cube.mesh");
		entity->setMaterialName("CrosshairMaterial");

		createPlayerShip();
		camera_node->setPosition(player_cameras[cameraToDisplay]->getPosition());

		targetCube = ship_node->createChildSceneNode("player_target.cube");
		targetCube->attachObject(entity);
		targetCube->translate(0.0f, 0.0f, -30.0f);

		std::cout << "Target Cube: " << targetCube ->_getDerivedPosition() << std::endl; 

		// Create Out Weapon Placements
		Ogre::Vector3 gunBase = player_cameras[cameraToDisplay]->getPosition();
		gunPlacements.push_back(Ogre::Vector3(-1.25f, 0, 4.0f));
		gunPlacements.push_back(Ogre::Vector3(1.25f, 0, 4.0f));
	}

	// Fires The Weapon If Hasn't Been Fired
	void Player::fireShot (void) {

		Ogre::Vector3 weapon_spawn_pos = ship_node->_getDerivedPosition() + (targetCube->_getDerivedPosition() -ship_node->_getDerivedPosition()).normalisedCopy() * 4.0;

		// Fire The Weapon
		if (weapons[currentWeaponIndex]->getOrientationNeeded()) {
			weapons[currentWeaponIndex]->fire_weapon(scene_manager, weapon_spawn_pos, 
				(targetCube->_getDerivedPosition() - ship_node->_getDerivedPosition()).normalisedCopy(), ship_node->getOrientation());
		}
		else {
			weapons[currentWeaponIndex]->fire_weapon(scene_manager, weapon_spawn_pos, (targetCube->_getDerivedPosition() -ship_node->_getDerivedPosition()).normalisedCopy(), getPlayerUpVector());
		}
	}

	float Player::getBoundingCircleRadius (void) {
		return COLLISION_DETECTION_RAD;
	}

	// Resets The Player When Called
	void Player::resetPosition(void) {
		camera_node->setPosition(initialPosition);
		camera_node->setOrientation(initialOrientation);

		player_cameras[cameraToDisplay]->setPosition(initialPosition);
		player_cameras[cameraToDisplay]->setOrientation(initialOrientation);

		health = PLAYER_STARTING_HEALTH;

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
		current_weapon_text = static_cast<Ogre::TextAreaOverlayElement*>(overlay_manager.createOverlayElement("TextArea", "MyTextArea"));
		current_weapon_text->setPosition(0, 90);
		current_weapon_text->setCaption("Current Weapon: " + weapons[currentWeaponIndex]->getWeaponName());
		initTextArea(current_weapon_text);
		panel->addChild(current_weapon_text);

		amount_of_enemies_text = static_cast<Ogre::TextAreaOverlayElement*>(overlay_manager.createOverlayElement("TextArea", "amount_of_enemies"));
		amount_of_enemies_text->setPosition(0, 60);
		amount_of_enemies_text->setCaption("Remaining Enemies: " + Ogre::StringConverter::toString(0));
		initTextArea(amount_of_enemies_text);
		panel->addChild(amount_of_enemies_text);

		current_player_health_text = static_cast<Ogre::TextAreaOverlayElement*>(overlay_manager.createOverlayElement("TextArea", "current_player_health"));
		current_player_health_text->setPosition(0, 30);
		current_player_health_text->setCaption("Current Health: " + Ogre::StringConverter::toString(health) + "/" + Ogre::StringConverter::toString(PLAYER_STARTING_HEALTH));
		initTextArea(current_player_health_text);
		panel->addChild(current_player_health_text);

		current_level_text = static_cast<Ogre::TextAreaOverlayElement*>(overlay_manager.createOverlayElement("TextArea", "current_level_text"));
		current_level_text->setPosition(0, 0);
		current_level_text->setCaption("Level = " + Ogre::StringConverter::toString(0) + "/" + Ogre::StringConverter::toString(PLAYER_STARTING_HEALTH));
		initTextArea(current_level_text);
		panel->addChild(current_level_text);

		// Create an overlay using the panel
		Ogre::Overlay* overlay = overlay_manager.create("MyOverlay");
		overlay->add2D(panel);
		overlay->show();
	}

	// Inits The Text Area
	void Player::initTextArea(Ogre::TextAreaOverlayElement* text_area) {
		text_area->setMetricsMode(Ogre::GMM_PIXELS);
		text_area->setDimensions(200, 100);
		text_area->setFontName("MyFont");
		text_area->setCharHeight(26);
		text_area->setColour(Ogre::ColourValue(0.8, 0.0, 0.0));
	}

	// Registers A Hit On The Player
	void Player::registerHit (int damageAmount) {
		health = std::max (health - damageAmount, 0);
		current_player_health_text->setCaption("Current Health: " + Ogre::StringConverter::toString(health) + "/" + Ogre::StringConverter::toString(PLAYER_STARTING_HEALTH));
		if (health == 0) {
			resetPosition();
		}
	}

	// Updates The Player UI When Called
	void Player::updatePlayerUI (int enemies_remaining, int level_num) {
		current_level_text->setCaption("Level = " + Ogre::StringConverter::toString(level_num + 1));
		amount_of_enemies_text->setCaption("Remaining Enemies: " + Ogre::StringConverter::toString(enemies_remaining));
	}

	/*------------------------------------------------------------- Getters And Setters ------------------------------------------------------------------*/
	Ogre::Vector3 Player::getPlayerDirection() {
		return (targetCube->_getDerivedPosition() - ship_node->_getDerivedPosition()).normalisedCopy();
	}

	Ogre::Vector3 Player::getPlayerUpVector() {
		return player_cameras[cameraToDisplay]->getUp();
	}

	Ogre::Vector3 Player::getDirection(){
		return player_cameras[cameraToDisplay]->getDirection();
	}

	Ogre::Vector3 Player::getPosition() {
		return ship_node->_getDerivedPosition();
	}

	/*-------------------------------------------------------- Ship Creation Functions --------------------------------------------------------------------*/
	void Player::createPlayerShip(void) {
		Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();
		Ogre::Entity *entity;
		Ogre::Matrix4 transformations;
		Ogre::String part_name;
		Ogre::String entity_name = "base_ship" + Ogre::StringConverter::toString("player");

		// First Create Our Camera Node
		camera_node = root_scene_node->createChildSceneNode("player.camera.node");

		// Now Create The Rest Of Ship
		entity = scene_manager->createEntity(entity_name, "cube.mesh");
		entity->setMaterialName("ShipTexture");
		ship_node = camera_node->createChildSceneNode(entity_name);
		ship_node->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(2.0, 2.0, 2.0));
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(0.0, 0.0, 0.0))) * transformations;
		AssignTransf(ship_node, transformations);

		part_name = "rightarm2";
		entity = scene_manager->createEntity(entity_name + part_name, "cube.mesh");  //mesh name on the right, entity on the left
		entity->setMaterialName("ShipTexture");
		Ogre::SceneNode* rightArm2 = ship_node->createChildSceneNode(entity_name + part_name);
		rightArm2->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(0.5, 0.5, 0.5));
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(-1.25, 0.0, -2.0))) * transformations;
		AssignTransf(rightArm2, transformations);

		part_name = "leftarm2";
		entity = scene_manager->createEntity(entity_name + part_name, "cube.mesh");  //mesh name on the right, entity on the left
		entity->setMaterialName("ShipTexture");
		Ogre::SceneNode* leftArm2 = ship_node->createChildSceneNode(entity_name + part_name);
		leftArm2->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(0.5, 0.5, 0.5));
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(1.25, 0.0, -2.0))) * transformations;
		AssignTransf(leftArm2, transformations);

		part_name = "rightcannon2";
		entity = scene_manager->createEntity(entity_name + part_name, "Cylinder");  //mesh name on the right, entity on the left
		Ogre::SceneNode* rightCannon = ship_node->createChildSceneNode(entity_name + part_name);
		rightCannon->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(0.25, 0.25, 0.25));
		transformations = Ogre::Matrix4(RotationMatrix(Ogre::Vector3(1.0, 0.0, 0.0), Ogre::Radian(Ogre::Math::PI/2.0))) * transformations;
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(-1.25, 0.0, -2.5))) * transformations;
		AssignTransf(rightCannon, transformations);

		part_name = "leftcannon2";
		entity = scene_manager->createEntity(entity_name + part_name, "Cylinder");  //mesh name on the right, entity on the left
		Ogre::SceneNode* leftCannon = ship_node->createChildSceneNode(entity_name + part_name);
		leftCannon->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(0.25, 0.25, 0.25));
		transformations = Ogre::Matrix4(RotationMatrix(Ogre::Vector3(1.0, 0.0, 0.0), Ogre::Radian(Ogre::Math::PI/2.0))) * transformations;
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(1.25, 0.0, -2.5))) * transformations;
		AssignTransf(leftCannon, transformations);

		part_name = "dashboard";
		entity = scene_manager->createEntity(entity_name + part_name, "cube.mesh");
		entity->setMaterialName("DashBoard");
		Ogre::SceneNode* dashboard = ship_node->createChildSceneNode(entity_name + part_name);
		dashboard->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(2.0, 0.35, 0.05));
		transformations = Ogre::Matrix4(RotationMatrix(Ogre::Vector3(-1.0, 0.0, 0.0), Ogre::Radian(Ogre::Math::PI/4))) * transformations;
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(0.0, -0.65, -2.0))) * transformations;
		AssignTransf(dashboard, transformations);

		part_name = "windowParamright";
		entity = scene_manager->createEntity(entity_name + part_name, "cube.mesh");
		entity->setMaterialName("ShipTexture");
		Ogre::SceneNode* windowParamright = ship_node->createChildSceneNode(entity_name + part_name);
		windowParamright->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(0.10, 1.5, 0.10));
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(1.1, 0.0, -2.0))) * transformations;
		AssignTransf(windowParamright, transformations);

		part_name = "windowParamleft";
		entity = scene_manager->createEntity(entity_name + part_name, "cube.mesh");
		entity->setMaterialName("ShipTexture");
		Ogre::SceneNode* windowParamleft = ship_node->createChildSceneNode(entity_name + part_name);
		windowParamleft->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(0.10, 1.5, 0.10));
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(-1.1, 0.0, -2.0))) * transformations;
		AssignTransf(windowParamleft, transformations);

		part_name = "windowParamtop";
		entity = scene_manager->createEntity(entity_name + part_name, "cube.mesh");
		entity->setMaterialName("ShipTexture");
		Ogre::SceneNode* windowParamtop = ship_node->createChildSceneNode(entity_name + part_name);
		windowParamtop->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(1.5, 0.10, 0.10));
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(0.0, 0.65, -1.5))) * transformations;
		AssignTransf(windowParamtop, transformations);

		part_name = "windowtopright";
		entity = scene_manager->createEntity(entity_name + part_name, "cube.mesh");
		entity->setMaterialName("ShipTexture");
		Ogre::SceneNode* windowtopright = ship_node->createChildSceneNode(entity_name + part_name);
		windowtopright->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(0.2, 0.3, 0.10));
		transformations = Ogre::Matrix4(RotationMatrix(Ogre::Vector3(0.0, 0.0, 1.0), Ogre::Radian(Ogre::Math::PI/4))) * transformations;
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(0.85, 0.6, -1.5))) * transformations;
		AssignTransf(windowtopright, transformations);

		part_name = "windowtopleft";
		entity = scene_manager->createEntity(entity_name + part_name, "cube.mesh");
		entity->setMaterialName("ShipTexture");
		Ogre::SceneNode* windowtopleft = ship_node->createChildSceneNode(entity_name + part_name);
		windowtopleft->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(0.2, 0.3, 0.10));
		transformations = Ogre::Matrix4(RotationMatrix(Ogre::Vector3(0.0, 0.0, -1.0), Ogre::Radian(Ogre::Math::PI/4))) * transformations;
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(-0.85, 0.6, -1.5))) * transformations;
		AssignTransf(windowtopleft, transformations);
	}
}