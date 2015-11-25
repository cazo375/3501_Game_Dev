#include <vector>
#include "OGRE/OgreRoot.h"
#include "OGRE/OgreRenderSystem.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreWindowEventUtilities.h"
#include "OGRE/OgreManualObject.h"
#include "OGRE/OgreEntity.h"
#include "OIS/OIS.h"

#ifndef EXPLOSION_H
#define EXPLOSION_H
#define EXPLOSION_LIFE_SPAN 2.0f
#define EXPLOSION_RADIUS 30.0f
namespace Explosion_Space {
	class Explosion {
	public:
		Explosion(void);
		Explosion(Ogre::SceneManager*, Ogre::Vector3);
		~Explosion(void);

		void advance (Ogre::Real);
		float getExplosionTimer(void);
		Ogre::Vector3 getPosition(void);
		boolean shouldDeleteExplosion (void);

	private:
		Ogre::Vector3 initialPos;
		Ogre::SceneNode* explosionNode;
		Ogre::SceneManager* scene_manager;
		Ogre::String explosion_name;

		float currentExplosionTimer;

		/*-------------------------------------- Functions -------------------------------*/
		void detonate(void);
		void updateMaterialWithTimer(Ogre::Real);
		void deleteExplosionFromScene(void);
	};
}

#endif