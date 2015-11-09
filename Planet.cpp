// Actual Implementation Of The Planet Object
#include "Planet.h"


// Namespace To Work In
namespace Planet_Space {

	static int planet_num;

	Planet::Planet(void) {
		rotationQuaternion = Ogre::Quaternion(Ogre::Radian (0.0002f), Ogre::Vector3(1.0f, 1.0f, 1.0f));
	}

	// Public Constructor For Our Planet
	Planet::Planet(Ogre::SceneManager* manager, PLANET_NAME name, Ogre::Vector3 pos) : planetType (name) {
		createPlanet(manager, pos);
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
		switch (planetType) {
		case EARTH :
			createPlanetOnType (manager, "Earth", "EarthTexture");
			radius = 50.0f;
			break;
		case VENUS :
			createPlanetOnType (manager, "Venus", "VenusTexture");
			radius = 40.0f;
			break;
		case JUPITER :
			createPlanetOnType (manager, "Jupiter", "JupiterTexture");
			radius = 150.0f;
			break;
		case NEPTUNE :
			createPlanetOnType (manager, "Neptune", "NeptuneTexture");
			radius = 100.0f;
			break;
		case NEBULA :
			createPlanetOnType (manager, "Nebula", "NebulaTexture", "reverse.sphere.mesh");
			radius = 3000.0f;
			break;
		}

		// Finally Scale Our Node Properly
		planetNode->scale(radius, radius, radius);
		planetNode->setPosition(pos);
	}

	// Creates the Planet Type
	void Planet::createPlanetOnType (Ogre::SceneManager* manager, Ogre::String entity_name, Ogre::String material_name, Ogre::String mesh_name) {
		planetName = Ogre::String(entity_name +  Ogre::StringConverter::toString(planet_num++));
		Ogre::Entity *entity = manager->createEntity(planetName, mesh_name);
		entity->setMaterialName(material_name);

		planetNode = manager->getRootSceneNode()->createChildSceneNode(planetName);
		planetNode->attachObject(entity);
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