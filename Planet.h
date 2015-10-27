// Simple Planet Object, That Will Be Rendered In The Game
#include "OGRE/OgreRoot.h"
#include "OGRE/OgreRenderSystem.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreWindowEventUtilities.h"
#include "OGRE/OgreManualObject.h"
#include "OGRE/OgreEntity.h"
#include "OIS/OIS.h"

enum PLANET_NAME {EARTH, VENUS, NEBULA};

namespace Planet_Space {
	class Planet {
	public:
		Planet(Ogre::SceneManager* manager, PLANET_NAME name = EARTH);
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
	};
}