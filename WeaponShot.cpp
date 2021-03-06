#include "WeaponShot.h"

namespace Weapon_Shot_Space {

	static int playerShot = 0;

	Weapon_Shot::Weapon_Shot (void) {
	}

	// Constructor For The Weapons Shot
	Weapon_Shot::Weapon_Shot(Ogre::Vector3 dir, Ogre::String shotName, int damage_amount) {
		lifeCounter = 0;
		boundingSphereRadius = 1;
		damageAmount = damage_amount;
		direction = dir;
		entity_name = shotName + "shot" + Ogre::StringConverter::toString(playerShot++);
		weapon_mesh = "cube.mesh";
		destory = false;
		weapon_shot_node = nullptr;
	}

	Weapon_Shot::~Weapon_Shot(void) {
		destoryFiredWeapon();
	}

	void Weapon_Shot::createEntity (Ogre::SceneManager* manager, Ogre::Vector3 position) {
		scene_manager = manager;

		// Create And Place The Obhect
		Ogre::Entity* entity = scene_manager->createEntity(entity_name, weapon_mesh);
		Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();
		weapon_shot_node = root_scene_node->createChildSceneNode(entity_name);
		weapon_shot_node->attachObject(entity);
		weapon_shot_node->setPosition(position);
		weapon_shot_node->setScale (0.50f, 0.50f, 0.5f);
	}

	// Moves Our Lazer If It Is Defined
	void Weapon_Shot::moveShot(Ogre::Real time) {
		if (weapon_shot_node) {
			weapon_shot_node->translate(direction * LAZER_THRUST);
			lifeCounter += time;

			if (lifeCounter >= LAZER_LIFE_SPAN) {
				destoryFiredWeapon();
			}
		}
	}

	// Destorys the lazer when called
	void Weapon_Shot::destoryFiredWeapon (void) {
		if (weapon_shot_node) {
			Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();
			root_scene_node->removeAndDestroyChild(weapon_shot_node->getName());
			scene_manager->destroyEntity(entity_name);
			weapon_shot_node = 0;
		}
	}

	// Returns Wheither Or Not The Shot ShouldBe 
	boolean Weapon_Shot::shouldDestoryShot (void) {
		return (lifeCounter >= LAZER_LIFE_SPAN) || destory;
	}

	Ogre::Vector3 Weapon_Shot::getPosition(void) {
		return weapon_shot_node->getPosition();
	}

	Ogre::Vector3 Weapon_Shot::getDirection(void) {
		return direction;
	}

	float Weapon_Shot::getDamageAmount(void) {
		return damageAmount;
	}

	float Weapon_Shot::getBoundingSphereRadius (void) {
		return boundingSphereRadius;
	}

	void Weapon_Shot::registerImpact(void) {
		destory = true;
	}

	/*------------------------------------------------- Exposive Shell ----------------------------------*/
	Explosive_Shot::Explosive_Shot (Ogre::Vector3 dir, Ogre::String shotName, int damage_amount) : Weapon_Shot (dir, shotName, damage_amount){
		exploded = false;
		explosion = nullptr;
		explosion_timer = 0;
	}

	Explosive_Shot::~Explosive_Shot(void) {
		Weapon_Shot::destoryFiredWeapon();

		if (explosion) {
			delete explosion;
			explosion = nullptr;
		}
	}

	// Only Destory The Shot When The Explosion Has Expired
	boolean Explosive_Shot::shouldDestoryShot (void) {
		return lifeCounter >= LAZER_LIFE_SPAN + EXPLOSION_LIFE_SPAN;
	}

	// Get The Position Of The Node... If Exploded Get Position Of The Explosion
	Ogre::Vector3 Explosive_Shot::getPosition (void) {
		Ogre::Vector3 returnVal = Ogre::Vector3 (0.0f);
		if (explosion) {
			returnVal = explosion->getPosition();
		} else if (weapon_shot_node) {
			returnVal = weapon_shot_node->getPosition();
		}
		return returnVal;
	}

	// Gets The Bounding Sphere Radius... If Exploding Factor That In
	float Explosive_Shot::getBoundingSphereRadius (void) {
		if (explosion) {
			return boundingSphereRadius + (explosion->getExplosionTimer() * EXPLOSION_RADIUS);
		}
		return boundingSphereRadius;
	}

	// Moves The Explosive Shot Through The World
	void Explosive_Shot::moveShot(Ogre::Real time) {
		lifeCounter += time;

		// If We Still Have Our Weapon
		if (weapon_shot_node) {
			weapon_shot_node->translate(direction * LAZER_THRUST);
			if (lifeCounter > EXPLOSION_DELAY && !exploded) {
				explodeRound();
			}
		}

		// If The Explosion Exists Then Progress
		if (explosion) {
			explosion->advance(time);
		}
	}

	void Explosive_Shot::registerImpact(void) {
		explodeRound();
	}

	// Explodes The Round When Called
	void Explosive_Shot::explodeRound (void) {
		if (!exploded) {
			exploded = true;
			explosion_timer = 0;
			explosion = new Explosion_Space::Explosion(scene_manager, weapon_shot_node->getPosition());
			destoryFiredWeapon();
		}
	}


	/*------------------------------------------------- Splinic Explosion Shell ----------------------------------*/
	Splinic_Shot::Splinic_Shot(Ogre::Vector3 init_pos, Ogre::Vector3 cross_product, Ogre::Vector3 init_direction, Ogre::String name, int damage)
		: Explosive_Shot (init_pos, name, damage) {
			points.push_back(init_pos);
			points.push_back(init_pos + (init_direction.normalisedCopy() * (SPLINE_FORWARD_INTERP / 2.0f)));
			points.push_back(init_pos + (init_direction.normalisedCopy() * (SPLINE_FORWARD_INTERP)) + (cross_product * SPLINE_SIDE_INTERP / 2.0f));
			points.push_back(init_pos + (init_direction.normalisedCopy() * (SPLINE_FORWARD_INTERP)) + (cross_product * SPLINE_SIDE_INTERP));
	}

	// Destroy The Splinic Shot
	Splinic_Shot::~Splinic_Shot(void){
		Weapon_Shot::destoryFiredWeapon();
		if (explosion) {
			delete explosion;
			explosion = nullptr;
		}
	}

	// Moves The Shot Accordingly
	void Splinic_Shot::moveShot(Ogre::Real time) {

		// If The Shot Still Exists
		if (weapon_shot_node) {
			lifeCounter += time;
			float percentProgressed = lifeCounter / LAZER_LIFE_SPAN;

			// Splinic Cofficents
			float p1w = std::pow((1 - percentProgressed), 3);
			float p2w = std::pow((1 - percentProgressed), 2) * 3 * percentProgressed;
			float p3w = std::pow(percentProgressed, 2) * (1 - percentProgressed) * 3;
			float p4w = std::pow(percentProgressed, 3);

			// Splinic Interpolation
			Ogre::Vector3 splinic_interp = p1w * points[0] + p2w * points[1] + p3w * points[2] + p4w * points[3];
			weapon_shot_node->setPosition(splinic_interp);

			if (lifeCounter > EXPLOSION_DELAY && !exploded) {
				explodeRound();
			}
		}

		// If The Explosion Exists Then Progress
		if (explosion) {
			explosion->advance(time);
		}
	}
}