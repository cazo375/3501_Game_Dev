#include "ogre_application.h"
#include "bin/path_config.h"

/*
this is a test//delete me

Player Ship Controls:
- W = Engage Forward Thruster
- S = Engage Back Thruster
- , = Roll Ship Left
- . = Roll Ship Right
- A = Engage left strafe thruster
- D = Enage right strafe thruster
- E = Move ship up
- X = Move ship down
- F = Fire Lazer
- Up Arrow: Rotate ship upwards
- Downward Arrow: Rotate ship downwards
- Left Arrow: Turn Ship Sideways
- Right Arrow: Turn Ship Right


Target Cube Controls:
- I = Move Target Cube Up
- J = Move Target Cube Left
- K = Move Target Cube Down
- L = Move Target Cube Right
*/

namespace ogre_application {

	/* Some configuration constants */
	/* They are written here as global variables, but ideally they should be loaded from a configuration file */

	/* Initialization */
	const Ogre::String config_filename_g = "";
	const Ogre::String plugins_filename_g = "";
	const Ogre::String log_filename_g = "Ogre.log";

	/* Main window settings */
	const Ogre::String window_title_g = "Demo";
	const Ogre::String custom_window_capacities_g = "";
	const unsigned int window_width_g = 800;
	const unsigned int window_height_g = 600;
	const bool window_full_screen_g = false;

	/* Viewport and camera settings */
	float viewport_width_g = 0.95f;
	float viewport_height_g = 0.95f;
	float viewport_left_g = (1.0f - viewport_width_g) * 0.5f;
	float viewport_top_g = (1.0f - viewport_height_g) * 0.5f;
	unsigned short viewport_z_order_g = 100;
	const Ogre::ColourValue viewport_background_color_g(0.0, 0.0, 0.0);
	float camera_near_clip_distance_g = 0.1;
	float camera_far_clip_distance_g = 5000.0;
	Ogre::Vector3 camera_position_g(0.0, 0.0, 800.0);
	Ogre::Vector3 targeting_cube_pos (0.0, 0.0, 750.0);
	Ogre::Vector3 camera_look_at_g(0.0, 0.0, 0.0);
	Ogre::Vector3 camera_up_g(0.0, 1.0, 0.0);

	/* Materials */
	const Ogre::String material_directory_g = MATERIAL_DIRECTORY;


	OgreApplication::OgreApplication(void) {

		/* Don't do work in the constructor, leave it for the Init() function */
	}


	void OgreApplication::Init(void){

		/* Set default values for the variables */
		animating_ = true;
		space_down_ = false;

		input_manager_ = NULL;
		keyboard_ = NULL;
		mouse_ = NULL;

		/* Camera demo */
		last_dir_ = Direction::Forward;

		currentForwardThrust = 0.0f;
		currentSideThrust = 0.0f;
		currentUpDownThrust = 0.0f;

		num_asteroids_ = 0;

		/* Run all initialization steps */
		InitRootNode();
		InitPlugins();
		InitRenderSystem();
		InitWindow();
		InitViewport();
		InitEvents();
		InitOIS();
		LoadMaterials();

		// Load Up Meshes
		Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
		Mesh_Manager_Space::Mesh_Manager::buildAllMeshes(scene_manager);
	}


	void OgreApplication::InitRootNode(void){

		try {

			/* We need to have an Ogre root to be able to access all Ogre functions */
			ogre_root_ = std::auto_ptr<Ogre::Root>(new Ogre::Root(config_filename_g, plugins_filename_g, log_filename_g));
			//ogre_root_->showConfigDialog();

		}
		catch (Ogre::Exception &e){
			throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
		}
		catch(std::exception &e){
			throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
		}
	}


	void OgreApplication::InitPlugins(void){

		try {

			/* Load plugin responsible for OpenGL render system */
			Strings plugin_names;
			plugin_names.push_back("RenderSystem_GL");

			Strings::iterator iter = plugin_names.begin();
			Strings::iterator iter_end = plugin_names.end();
			for (; iter != iter_end; iter++){
				Ogre::String& plugin_name = (*iter);
				if (OGRE_DEBUG_MODE){
					plugin_name.append("_d");
				}
				ogre_root_->loadPlugin(plugin_name);
			}

		}
		catch (Ogre::Exception &e){
			throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
		}
		catch(std::exception &e){
			throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
		}
	}


	void OgreApplication::InitRenderSystem(void){

		try {

			const Ogre::RenderSystemList& render_system_list = ogre_root_->getAvailableRenderers();
			if (render_system_list.size() == 0)
			{
				throw(OgreAppException(std::string("OgreApp::Exception: Sorry, no rendersystem was found.")));
			}

			Ogre::RenderSystem *render_system = render_system_list.at(0);
			ogre_root_->setRenderSystem(render_system);

		}
		catch (Ogre::Exception &e){
			throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
		}
		catch(std::exception &e){
			throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
		}
	}


	void OgreApplication::InitWindow(void){

		try {

			/* Create main window for the application */
			bool create_window_automatically = false;
			ogre_root_->initialise(create_window_automatically, window_title_g, custom_window_capacities_g);

			Ogre::NameValuePairList params;
			params["FSAA"] = "0";
			params["vsync"] = "true";
			ogre_window_ = ogre_root_->createRenderWindow(window_title_g, window_width_g, window_height_g, window_full_screen_g, &params);

			ogre_window_->setActive(true);
			ogre_window_->setAutoUpdated(false);
		}
		catch (Ogre::Exception &e){
			throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
		}
		catch(std::exception &e){
			throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
		}
	}


	void OgreApplication::InitViewport(void){

		try {

			/* Retrieve scene manager and root scene node */
			Ogre::SceneManager* scene_manager = ogre_root_->createSceneManager(Ogre::ST_GENERIC, "MySceneManager");
			Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

			/* Create camera object */
			Ogre::Camera* camera = scene_manager->createCamera("MyCamera");
			Ogre::SceneNode* camera_scene_node = root_scene_node->createChildSceneNode("MyCameraNode");
			camera_scene_node->attachObject(camera);

			camera->setNearClipDistance(camera_near_clip_distance_g);
			camera->setFarClipDistance(camera_far_clip_distance_g); 

			camera->setPosition(camera_position_g);
			camera->lookAt(camera_look_at_g);
			camera->setFixedYawAxis(true, camera_up_g);

			/* Create viewport */
			Ogre::Viewport *viewport = ogre_window_->addViewport(camera, viewport_z_order_g, viewport_left_g, viewport_top_g, viewport_width_g, viewport_height_g);

			viewport->setAutoUpdated(true);
			viewport->setBackgroundColour(viewport_background_color_g);

			/* Set aspect ratio */
			float ratio = float(viewport->getActualWidth()) / float(viewport->getActualHeight());
			camera->setAspectRatio(ratio);
		}
		catch (Ogre::Exception &e){
			throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
		}
		catch(std::exception &e){
			throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
		}
	}


	void OgreApplication::InitEvents(void){

		try {

			/* Add this object as a FrameListener for render events (see frameRenderingQueued event) */
			ogre_root_->addFrameListener(this);

			/* Add this object as a WindowEventListener to handle the window resize event */
			Ogre::WindowEventUtilities::addWindowEventListener(ogre_window_, this);

		}
		catch (Ogre::Exception &e){
			throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
		}
		catch(std::exception &e){
			throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
		}
	}


	void OgreApplication::InitOIS(void){

		/* Initialize the Object Oriented Input System (OIS) */
		try {

			/* Initialize input manager */
			OIS::ParamList pl; // Parameter list passed to the input manager initialization
			size_t windowHnd = 0;
			std::ostringstream windowHndStr;
			ogre_window_->getCustomAttribute("WINDOW", &windowHnd);
			windowHndStr << windowHnd;
			pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
			pl.insert(std::make_pair(std::string("w32_mouse"), 
				std::string("DISCL_FOREGROUND" )));
			pl.insert(std::make_pair(std::string("w32_mouse"), 
				std::string("DISCL_NONEXCLUSIVE")));
			input_manager_ = OIS::InputManager::createInputSystem(pl);

			/*size_t hWnd = 0;
			ogre_window_->getCustomAttribute("WINDOW", &hWnd);
			input_manager_ = OIS::InputManager::createInputSystem(hWnd);*/

			/* Initialize keyboard and mouse */
			keyboard_ = static_cast<OIS::Keyboard*>(input_manager_->createInputObject(OIS::OISKeyboard, false));

			mouse_ = static_cast<OIS::Mouse*>(input_manager_->createInputObject(OIS::OISMouse, false));
			unsigned int width, height, depth;
			int top, left;
			ogre_window_->getMetrics(width, height, depth, left, top);
			const OIS::MouseState &ms = mouse_->getMouseState();
			ms.width = width;
			ms.height = height;

		}
		catch(std::exception &e){
			throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
		}
	}


	void OgreApplication::LoadMaterials(void){

		try {

			/* Load materials that can then be assigned to objects in the scene */
			Ogre::String resource_group_name = "MyGame";
			Ogre::ResourceGroupManager& resource_group_manager = Ogre::ResourceGroupManager::getSingleton();
			resource_group_manager.createResourceGroup(resource_group_name);
			bool is_recursive = false;
			resource_group_manager.addResourceLocation(material_directory_g, "FileSystem", resource_group_name, is_recursive);
			resource_group_manager.initialiseResourceGroup(resource_group_name);
			resource_group_manager.loadResourceGroup(resource_group_name);

		}
		catch (Ogre::Exception &e){
			throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
		}
		catch(std::exception &e){
			throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
		}
	}

	void OgreApplication::MainLoop(void){

		try {

			/* Main loop to keep the application going */

			ogre_root_->clearEventTimes();

			while(!ogre_window_->isClosed()){
				ogre_window_->update(false);

				ogre_window_->swapBuffers();

				ogre_root_->renderOneFrame();

				Ogre::WindowEventUtilities::messagePump();
			}
		}
		catch (Ogre::Exception &e){
			std::cout << "Exception: " << std::string(e.what());
			throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
		}
		catch(std::exception &e){
			std::cout << "Exception: " << std::string(e.what());
			throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
		}
	}


	bool OgreApplication::frameRenderingQueued(const Ogre::FrameEvent& fe){

		/* This event is called after a frame is queued for rendering */
		/* Do stuff in this event since the GPU is rendering and the CPU is idle */

		/* Keep animating if flag is on */
		if (animating_){
			/* Animate transformation */
			TransformAsteroidField();
			TransformPlanetField();
			level_manager.advanceCurrentLevel();
			player->advance();
		}

		/* Capture input */
		keyboard_->capture();
		mouse_->capture();

		/* Handle specific key events */
		if (keyboard_->isKeyDown(OIS::KC_SPACE)){
			space_down_ = true;
		}
		if ((!keyboard_->isKeyDown(OIS::KC_SPACE)) && space_down_){
			animating_ = !animating_;
			space_down_ = false;
		}
		if (keyboard_->isKeyDown(OIS::KC_ESCAPE)){
			ogre_root_->shutdown();
			ogre_window_->destroy();
			return false;
		}
		
		// Apply The Keyboard Command
		player->applyKeyEvent(keyboard_);

		
		// Switch Level Commands
		if (keyboard_->isKeyDown (OIS::KC_L)) {
			if (level_manager.canSwitchLevels()) {
				Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
				level_manager.cycleNextLevel(scene_manager);
			}
		}
		
		// Run All Of The Collision Dection
		runCollisionDetection();

		// Have The Enemies Fire At The Player
		haveEnemiesShootAtPlayer();

		// Increment The Level Ticker
		level_manager.incrementLevelTicker();

		return true;
	}

	// Have The Enemies Shoot At The Player
	void OgreApplication::haveEnemiesShootAtPlayer(void) {
		Level_Space::Level* currentLevel = level_manager.getCurrentLevelObj();
		std::vector<Enemy_Space::Enemy*> enemies = currentLevel->getEnemies();
	
		for (int i = 0; i < enemies.size(); i++) {
			enemies[i]->shouldFireShot(player);
		}
	}

	// Runs The Collision Detection 
	void OgreApplication::runCollisionDetection(void) {
		runLazerCollisionDetection();
		runPlanetCollisionDetection();
	}

	// Runs the Lazer Collision Detection
	void OgreApplication::runLazerCollisionDetection(void) {
		Weapon_Space::Weapon_Shot* lazer = player->getCurrentShot();
		if (lazer) {
			// Check Against Our Astorids
			for (int i = 0; i < num_asteroids_; i++){
				if (cube_[i]) {
					if (Collision_Manager::CollisionManager::runRaySphereCollision (lazer->getPosition(), lazer->getDirection(), asteroid_[i].pos, 8.0f)) {
						destoryAstroid(i);
						break;
					}
				}
			}		

			// Check Against Our Enemies
			Level_Space::Level* currentLevel = level_manager.getCurrentLevelObj();
			if (currentLevel) {
				std::vector<Enemy_Space::Enemy*> enemies = currentLevel->getEnemies();
				for (int i = 0; i < enemies.size(); i++) {
					Enemy_Space::Enemy* nextEnemy = enemies[i];
					if (Collision_Manager::CollisionManager::runBoundingSphereCollision (nextEnemy->getPosition(), lazer->getPosition(), nextEnemy->getBoundingCircleRadius(), 1.0f)) {
						nextEnemy->registerHit(lazer->getDamageAmount());

						// If The Enemy Was Killed Remove Them From The Level
						if (nextEnemy ->enemyDead()) {
							Level_Space::Level* currentLevel = level_manager.getCurrentLevelObj();
							if (currentLevel) {
								currentLevel->destoryEnemyAt(i);
							}
						}
						break;
					}
				}
			}
		}
	}

	// Runs The Planet Collsion Detection
	void OgreApplication::runPlanetCollisionDetection(void) {
		Level_Space::Level* currentLevel = level_manager.getCurrentLevelObj();
		if (currentLevel) {
			std::vector<Planet_Space::Planet*> planets = currentLevel->getPlanets();
			std::vector<Planet_Space::Planet*>::iterator iter = planets.begin();
			std::vector<Planet_Space::Planet*>::iterator iter_end = planets.end();
			for (; iter != iter_end; iter++){
				Planet_Space::Planet* nextPlanet = (*iter);
				if (Collision_Manager::CollisionManager::runBoundingSphereCollision (nextPlanet->getPlanetPostion(), player->getPosition(), nextPlanet->getPlanetRadius(), player->getBoundingCircleRadius())) {
					player->resetPosition();
					break;
				}
			}
		}

		// If The Player Escapes The Nebula
		if (!Collision_Manager::CollisionManager::runBoundingSphereCollision (nebula.getPlanetPostion(), player->getPosition(), nebula.getPlanetRadius() - 1200, player->getBoundingCircleRadius())) {
			player->resetPosition();
		}
	}

	void OgreApplication::windowResized(Ogre::RenderWindow* rw){

		/* Update the window and aspect ratio when the window is resized */
		int width = rw->getWidth(); 
		int height = rw->getHeight();

		Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
		Ogre::Camera* camera = scene_manager->getCamera("MyCamera");

		if (camera != NULL){
			camera->setAspectRatio((double)width/height);
		}

		const OIS::MouseState &ms = mouse_->getMouseState();
		ms.width = width;
		ms.height = height;

		ogre_window_->resize(width, height);
		ogre_window_->windowMovedOrResized();
		ogre_window_->update();
	}

	void OgreApplication::destoryAstroid(int index) {
		if (cube_[index]) {
			Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
			Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();
			root_scene_node->removeAndDestroyChild(cube_[index]->getName());

			cube_[index]= 0;
		}
	}

	// Creates The Planetary Field
	void OgreApplication::CreatePlanetField(void) {
		Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
		nebula = Planet_Space::Planet(scene_manager, NEBULA);
	}

	void OgreApplication::CreateAsteroidField(int num_asteroids){

		try {
			/* Check number of asteroids requested */
			if (num_asteroids > MAX_NUM_ASTEROIDS){
				num_asteroids_ = MAX_NUM_ASTEROIDS;
			} else {
				num_asteroids_ = num_asteroids;
			}

			/* Create asteroid field */
			for (int i = 0; i < num_asteroids_; i++){
				asteroid_[i].pos = Ogre::Vector3(-300 + 600 * (rand() % 1000) / 1000.0f, -300 + 600 * (rand() % 1000) / 1000.0f, 600 * (rand() % 1000) / 1000.0f);
				asteroid_[i].ori = Ogre::Quaternion(1.0f, 3.14*(rand() % 1000) / 1000.0f, 3.14*(rand() % 1000) / 1000.0f, 3.14*(rand() % 1000) / 1000.0f);
				asteroid_[i].lm = Ogre::Quaternion(1.0f, 0.005*3.14*(rand() % 1000) / 1000.0f, 0.005*3.14*(rand() % 1000) / 1000.0f, 0.005*3.14*(rand() % 1000) / 1000.0f);
				asteroid_[i].drift = Ogre::Vector3(((double) rand() / RAND_MAX)*0.2, ((double) rand() / RAND_MAX)*0.2, ((double) rand() / RAND_MAX)*0.2);
			}

			/* Create multiple entities for the asteroids */

			/* Retrieve scene manager and root scene node */
			Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
			Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

			/* Create multiple entities of a mesh */
			Ogre::String entity_name, prefix("Asteroid");
			for (int i = 0; i < num_asteroids_; i++){

				/* Create entity */
				entity_name = prefix + Ogre::StringConverter::toString(i);
				//Ogre::Entity *entity = scene_manager->createEntity(entity_name, "Cube");
				Ogre::Entity *entity = scene_manager->createEntity(entity_name, "Icosahedron");
				entity->setMaterialName("ShinyBlueMaterial");

				/* Create a scene node for the entity */
				/* The scene node keeps track of the entity's position */
				cube_[i] = root_scene_node->createChildSceneNode(entity_name);
				cube_[i]->attachObject(entity);
			}

		}
		catch (Ogre::Exception &e){
			throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
		}
		catch(std::exception &e){
			throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
		}
	}



	void OgreApplication::TransformAsteroidField(void){

		// Rotate asteroids
		for (int i = 0; i < num_asteroids_; i++){

			if (cube_[i]) {
				// Set orientation
				asteroid_[i].ori = asteroid_[i].lm * asteroid_[i].ori;
				cube_[i]->setOrientation(asteroid_[i].ori);

				// Set the position every time
				cube_[i]->setPosition(asteroid_[i].pos);
			}
		}
	}

	// Transform Our Planet Field
	void OgreApplication::TransformPlanetField(void) {
		nebula.advance();
	}

	// Fires The Entire Game Off...
	void OgreApplication::startGame(void) {
		Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
		Ogre::Camera* camera = scene_manager->getCamera("MyCamera");

		level_manager.cycleNextLevel(scene_manager);

		player = new Player_Space::Player (scene_manager, camera);
		player->createPlayer();
	}

} // namespace ogre_application;