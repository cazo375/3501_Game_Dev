#include <vector>
#include "OGRE/OgreRoot.h"
#include "OGRE/OgreRenderSystem.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreWindowEventUtilities.h"
#include "OGRE/OgreManualObject.h"
#include "OGRE/OgreEntity.h"
#include "OIS/OIS.h"

#ifndef ASTEROID_H
#define ASTEROID_H

namespace Asteroid_Space {
	class Asteroid {
	public:
		Asteroid(void);
		Asteroid(Ogre::SceneManager*);
		~Asteroid(void);

		void advance(void);
		Ogre::Vector3  getPosition(void);
	private:
		Ogre::Vector3 pos;			// Position
		Ogre::Quaternion ori;		// Orientation
		Ogre::Quaternion lm;		// Angular momentum (use as velocity)
		Ogre::Vector3 drift;		// Drift direction
		Ogre::String asteroidName;
		Ogre::SceneNode* asteroidNode;
		Ogre::SceneManager* scene_manager;

		void deleteAsteroidFromScene (void);
		void createAsteroid (Ogre::Vector3 pos);
	};
}

#endif