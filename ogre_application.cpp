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
- Q = Move ship down
- F = Fire Lazer
- C = Cycle Weapons
- L = Next Level
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

		currentForwardThrust = 0.0f;
		currentSideThrust = 0.0f;
		currentUpDownThrust = 0.0f;

		/* Run all initialization steps */
		InitRootNode();
		InitPlugins();
		InitRenderSystem();
		InitWindow();
		InitViewport();
		InitEvents();
		InitOIS();
		LoadMaterials();
		// InitOverlay();

		// Load Up Meshes
		Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
		Mesh_Manager_Space::Mesh_Manager::buildAllMeshes(scene_manager);
	}

	void OgreApplication::InitOverlay(void){

		// Create and initialize the overlay system
		Ogre::OverlaySystem *os = new Ogre::OverlaySystem();
		Ogre::SceneManager* scene_manager =   ogre_root_->getSceneManager("MySceneManager");
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
		Ogre::TextAreaOverlayElement* text_area = static_cast<Ogre::TextAreaOverlayElement*>(overlay_manager.createOverlayElement("TextArea", "MyTextArea"));
		text_area->setMetricsMode(Ogre::GMM_PIXELS);
		text_area->setPosition(0, 0);
		text_area->setDimensions(200, 100);
		text_area->setFontName("MyFont");
		text_area->setCaption("Hello World");
		text_area->setCharHeight(26);
		text_area->setColour(Ogre::ColourValue(0.8, 0.0, 0.0));
		panel->addChild(text_area);

		// Create an overlay using the panel
		Ogre::Overlay* overlay = overlay_manager.create("MyOverlay");
		overlay->add2D(panel);
		overlay->show();
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

	Ogre::Vector3 OgreApplication::getVectorFromTwoPoints(Ogre::Vector3 playerpos, Ogre::Vector3 Enemypos){
		Ogre::Vector3  playerdirection = (playerpos - Enemypos);
		return playerdirection;
	}


	// Create a rotation matrix based on an angle and an axis
	Ogre::Matrix4 OgreApplication::RotationMatrix(Ogre::Vector3 axis, Ogre::Radian angle){

		Ogre::Matrix3 mat;
		mat = Ogre::Matrix3::IDENTITY;
		mat.FromAngleAxis(axis, angle);
		return Ogre::Matrix4(mat);
	}


	// Create a translation matrix based on a vector of translations (x, y, z)
	Ogre::Matrix4 OgreApplication::TranslationMatrix(Ogre::Vector3 trans){

		Ogre::Matrix4 mat;
		mat = Ogre::Matrix4::IDENTITY;
		mat.setTrans(trans);
		return mat;
	}


	// Create a scaling matrix based on a vector of scale factors (x, y, z)
	Ogre::Matrix4 OgreApplication::ScalingMatrix(Ogre::Vector3 scale){

		Ogre::Matrix4 mat;
		mat = Ogre::Matrix4::IDENTITY;
		mat.setScale(scale);
		return mat;
	}

	void OgreApplication::AssignTransf(Ogre::SceneNode* node, Ogre::Matrix4 transf){

		/* In many graphic frameworks, we would simply multiply our geometry by the transformation matrix.
		However, OGRE stores the transformations of a node in a more efficient manner.
		So, we need to decompose the transformation first into three components and then assign them
		to the scene node.*/
		Ogre::Vector3 trans, scale;
		Ogre::Quaternion quat;

		transf.decomposition(trans, scale, quat);
		node->setScale(scale);
		node->setOrientation(quat);
		node->setPosition(trans);
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
			TransformPlanetField();
			level_manager.advanceCurrentLevel(player);
			player->advance(fe.timeSinceLastFrame);
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

		// Advance The Explosions
		advanceAllExplosions(fe.timeSinceLastFrame);
		deleteAllExpiredExplosions();

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
		std::vector<Weapon_Shot_Space::Weapon_Shot*> lazers = player->getCurrentShots();

		for (int i = 0; i < lazers.size(); i++) {
			Weapon_Shot_Space::Weapon_Shot* lazer = lazers[i];

			if (lazer) {

				// Check Against Our Astorids
				Level_Space::Level* currentLevel = level_manager.getCurrentLevelObj();

				if (currentLevel){
					Ogre::Vector3 lazerPosition = lazer->getPosition();
					float lazerBoundingSphereRadius = lazer->getBoundingSphereRadius();

					// Check Against Our Asteroids
					std::vector<Asteroid_Space::Asteroid*> levelAsteroids = currentLevel->getAsteroids();
					for (int i = 0; i < levelAsteroids.size(); i++){
						if (Collision_Manager::CollisionManager::runBoundingSphereCollision (lazerPosition, levelAsteroids[i]->getPosition(), lazerBoundingSphereRadius, 8.0f)) {
							lazer->registerImpact();
							spawnExplosionAt(levelAsteroids[i]->getPosition());
							currentLevel->destoryAsteroidAt(i);
							break;
						}
					}		

					// Check Against Our Enemies
					std::vector<Enemy_Space::Enemy*> enemies = currentLevel->getEnemies();
					for (int i = 0; i < enemies.size(); i++) {
						Enemy_Space::Enemy* nextEnemy = enemies[i];
						if (Collision_Manager::CollisionManager::runBoundingSphereCollision (nextEnemy->getPosition(), lazerPosition, nextEnemy->getBoundingCircleRadius(), lazerBoundingSphereRadius)) {
							nextEnemy->registerHit(lazer->getDamageAmount());

							// If The Enemy Was Killed Remove Them From The Level
							if (nextEnemy ->enemyDead()) {
								Level_Space::Level* currentLevel = level_manager.getCurrentLevelObj();
								if (currentLevel) {
									lazer->registerImpact();
									spawnExplosionAt (nextEnemy->getPosition());
									currentLevel->destoryEnemyAt(i);
								}
							}
							break;
						}
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
		if (!Collision_Manager::CollisionManager::runBoundingSphereCollision (nebula.getPlanetPostion(), player->getPosition(), nebula.getPlanetRadius() - 600, player->getBoundingCircleRadius())) {
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

	// Creates The Planetary Field
	void OgreApplication::CreatePlanetField(void) {
		Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
		nebula = Planet_Space::Planet(scene_manager, NEBULA);
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

	/*-------------------------------------- Explosion Methods --------------------------------*/
	void OgreApplication::spawnExplosionAt (Ogre::Vector3 pos) {
		Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
		active_explosions.push_back(new Explosion_Space::Explosion (scene_manager, pos));
	}

	void OgreApplication::deleteAllExpiredExplosions (void) {
		for (int i = 0; i < active_explosions.size(); i++ ) {
			if (active_explosions[i]->shouldDeleteExplosion()) {
				delete active_explosions[i];
				active_explosions.erase(active_explosions.begin() + i);
				break;
			}
		}
	}

	// Moves The Explosions Along When Called
	void OgreApplication::advanceAllExplosions (Ogre::Real time) {
		for (int i = 0; i < active_explosions.size(); i++) {
			active_explosions[i]->advance(time);
		}
	}

} // namespace ogre_application;