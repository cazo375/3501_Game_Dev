// Simple Planet Object, That Will Be Rendered In The Game
#include "OGRE/OgreRoot.h"
#include "OGRE/OgreRenderSystem.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreWindowEventUtilities.h"
#include "OGRE/OgreManualObject.h"
#include "OGRE/OgreEntity.h"
#include "OIS/OIS.h"

#define NEBULA_RADIUS 1500.0f

enum PLANET_NAME {EARTH, VENUS, JUPITER, NEPTUNE, NEBULA};

// Planet Space
namespace Planet_Space {
	class Planet {
	public:
		Planet(void);
		Planet(Ogre::SceneManager* manager, PLANET_NAME name = EARTH, Ogre::Vector3 pos = Ogre::Vector3 (0.0, 0.0, 0.0));
		~Planet(void);

		// Getters And Setters For This Planet Object
		float getPlanetRadius (void);
		Ogre::Vector3 getPlanetPostion (void);
		void setPlanetPosition (Ogre::Vector3 value);
		void destoryPlanet (Ogre::SceneManager* scene_manager);

		// Methods For Updating Planets On A Per-Frame Basis
		void advance(void);
	private:
		Ogre::String planetName;
		Ogre::SceneNode* planetNode;
		Ogre::Quaternion rotationQuaternion;

		PLANET_NAME planetType;
		float radius;

		// Methods For Rendering The New Planet Object
		void createPlanet(Ogre::SceneManager* manager, Ogre::Vector3 pos = Ogre::Vector3 (0.0f, 0.0f, 0.0f));
		void createPlanetOnType (Ogre::SceneManager*, Ogre::String, Ogre::String, Ogre::String mesh = "sphere.mesh");
	};
}