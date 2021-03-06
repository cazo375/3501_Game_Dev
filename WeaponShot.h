/*
Our Generic Weapon Class. Holds The Data That Is Relevant To The Weapons.
*/

#include "OGRE/OgreRoot.h"
#include "OGRE/OgreRenderSystem.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreWindowEventUtilities.h"
#include "OGRE/OgreManualObject.h"
#include "OGRE/OgreEntity.h"
#include "OIS/OIS.h"
#include <vector>

#include "Explosion.h"

#ifndef WeaponBase_H

#define WeaponBase_H
#define LAZER_THRUST 6.0f
#define LAZER_LIFE_SPAN 0.5f
#define EXPLOSION_DELAY 0.5f

// Spline Stuff
#define SPLINE_FORWARD_INTERP 100.0f
#define SPLINE_SIDE_INTERP 30.0f

namespace Weapon_Shot_Space {

	/*------------------------------------------------- Basic Weapon Shot ---------------------------------------------------------------*/
	class Weapon_Shot {
	public:
		Weapon_Shot(void);
		Weapon_Shot(Ogre::Vector3 direction, Ogre::String, int damage = 1);
		virtual ~Weapon_Shot(void);

		// Public Methods For Moving The Shots In The Game
		void destoryFiredWeapon (void);

		// Getters And Setters For The Object
		Ogre::Vector3 getDirection (void);
		float getDamageAmount (void);

		// Virtual Functions
		virtual void registerImpact (void);
		virtual boolean shouldDestoryShot (void);
		virtual Ogre::Vector3 getPosition (void);
		virtual void moveShot (Ogre::Real time = 0);
		virtual float getBoundingSphereRadius (void);

		// For Actually Creating Our Entity
		void createEntity (Ogre::SceneManager* manager, Ogre::Vector3 pos);

	protected:

		// Variables For Creation
		Ogre::String weapon_mesh;
		Ogre::String entity_name;
		Ogre::SceneNode* weapon_shot_node;
		Ogre::SceneManager* scene_manager;

		// Identifer Variables
		Ogre::String weapon_name;

		// Values That Need To Be Know For Where The Object Is Going
		Ogre::Vector3 direction;
		Ogre::Quaternion ori;

		// Values For Movement
		float lifeCounter;
		float damageAmount;
		float boundingSphereRadius;

		// Booleans
		boolean destory;
	};

	/*------------------------------------------------- Explosive Shot ---------------------------------------------------------------*/
	class Explosive_Shot : public Weapon_Shot {
	public:
		Explosive_Shot(void);
		Explosive_Shot(Ogre::Vector3, Ogre::String, int damage = 1);
		virtual ~Explosive_Shot(void);

		void moveShot (Ogre::Real time = 0);
		void registerImpact (void);
		boolean shouldDestoryShot (void);

		Ogre::Vector3 getPosition (void);
		float getBoundingSphereRadius (void);
	protected:
		boolean exploded;
		float explosion_timer;
		Explosion_Space::Explosion* explosion;

		// Functions
		void explodeRound (void);
	};

	/*------------------------------------------------- Splinic Shot ---------------------------------------------------------------*/
	class Splinic_Shot :  public Explosive_Shot {
	public:
		Splinic_Shot(void);
		Splinic_Shot(Ogre::Vector3 init_pos, Ogre::Vector3 cross_product, Ogre::Vector3 init_direction, Ogre::String, int damage = 1);
		~Splinic_Shot(void);

		void moveShot (Ogre::Real time = 0);
	protected:
		std::vector<Ogre::Vector3> points;
	};
}

#endif