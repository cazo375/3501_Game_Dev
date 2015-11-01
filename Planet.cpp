// Actual Implementation Of The Planet Object
#include "Planet.h"


// Namespace To Work In
namespace Planet_Space {

	static int planet_num;

	Planet::Planet(void) {
		rotationQuaternion = Ogre::Quaternion(Ogre::Radian (0.0002f), Ogre::Vector3(1.0f, 1.0f, 1.0f));
	}

	// Public Constructor For Our Planet
	Planet::Planet(Ogre::SceneManager* manager, PLANET_NAME name) : planetType (name) {
		createPlanet(manager);
		rotationQuaternion = Ogre::Quaternion(Ogre::Radian (0.0002f), Ogre::Vector3(1.0f, 1.0f, 1.0f));
	}

	// Deconstructor
	Planet::~Planet(void) {
	}

	float Planet::getPlanetRadius (void) {
		return radius;
	}

	Ogre::Vector3 Planet::getPlanetPostion (void) {
		return planetNode->getPosition();
	}

	void Planet::setPlanetPosition (Ogre::Vector3 value) {
		if (planetNode) {
			planetNode->setPosition(value);
		}
	}

	// Creates Our Planet Entity
	void Planet::createPlanet(Ogre::SceneManager* manager, Ogre::Vector3 pos) {
		Ogre::Entity *entity;

		switch (planetType) {
		case EARTH :
			planetName = Ogre::String("Earth" + planet_num++);
			entity = manager->createEntity(planetName, "sphere.mesh");
			entity->setMaterialName("EarthTexture");

			planetNode = manager->getRootSceneNode()->createChildSceneNode(planetName);
			planetNode->attachObject(entity);

			radius = 50.0f;
			break;
		case VENUS :
			planetName = Ogre::String("Venus" + planet_num++);
			entity = manager->createEntity(planetName, "sphere.mesh");
			entity->setMaterialName("VenusTexture");

			planetNode = manager->getRootSceneNode()->createChildSceneNode(planetName);
			planetNode->attachObject(entity);

			radius = 40.0f;
			break;
		case NEBULA :
			planetName = Ogre::String("Nebula" + planet_num++);
			entity = manager->createEntity(planetName, "reverse.sphere.mesh");
			entity->setMaterialName("NebulaTexture");

			planetNode = manager->getRootSceneNode()->createChildSceneNode(planetName);
			planetNode->attachObject(entity);

			radius = 3000.0f;
			break;
		}

		// Finally Scale Our Node Properly
		planetNode->scale(radius, radius, radius);
	}

	// Advances Planet By A Step In Animation When Called.
	void Planet::advance(void) {
		planetNode->rotate(rotationQuaternion);
	}

	// Destorys Our Planet When Called
	void Planet::destoryPlanet (Ogre::SceneManager* scene_manager) {
		Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();
		root_scene_node->removeAndDestroyChild(planetNode->getName());
		scene_manager->destroyEntity(planetName);
		planetNode = 0;
	}
}