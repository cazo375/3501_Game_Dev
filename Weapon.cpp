#include "Weapon.h"

namespace Weapon_Space {
	BaseWeapon::BaseWeapon (void) {
		weapon_damage = 0;
		fireRate = 0;
		timeRemainingBeforeShot = 0;
		weapon_name = "base_weapon";
		owner_object = "unknown";
	}

	BaseWeapon::~BaseWeapon (void) {
	}

	// Moves All Shots By Another Frame
	void BaseWeapon::advance (Ogre::Real time) {
		std::vector<int> indexesToRemove;

		// Fire The Shots We Need
		for (int i = 0; i < shotsFired.size(); i++) {
			shotsFired[i]->moveShot(time);
			if (shotsFired[i]->shouldDestoryShot()) {
				shotsFired[i]->destoryFiredWeapon();
				indexesToRemove.push_back(i);
			}
		}

		// Delete The Shots That Have Expired From The Array
		int deleteAmount = 0;
		for (int i = 0; i < indexesToRemove.size(); i++) {
			delete shotsFired[indexesToRemove[i] - deleteAmount];
			shotsFired.erase(shotsFired.begin() + (indexesToRemove[i] - deleteAmount));
			deleteAmount++; 
		}

		if (timeRemainingBeforeShot <= fireRate) {
			timeRemainingBeforeShot++;
		}
	}

	// Returns The Number Of Shots Fired
	std::vector<Weapon_Shot_Space::Weapon_Shot*> BaseWeapon::getShotsFired(void) {
		return shotsFired;
	}

	// Returns The Weapons Name
	Ogre::String BaseWeapon::getWeaponName(void) {
		return weapon_name;
	}

	boolean BaseWeapon::getOrientationNeeded(void) {
		return needsOrientation;
	}

	// Fires The Weapon When Called
	void BaseWeapon::fire_weapon(Ogre::SceneManager* man, Ogre::Vector3 pos, Ogre::Vector3 dir) {
		if (timeRemainingBeforeShot > fireRate) {
			Weapon_Shot_Space::Weapon_Shot* newShot = new Weapon_Shot_Space::Weapon_Shot (dir, owner_object, weapon_damage);
			newShot->createEntity(man, pos);
			shotsFired.push_back(newShot);
			timeRemainingBeforeShot = 0;
		}
	}

	void BaseWeapon::fire_weapon(Ogre::SceneManager* man, Ogre::Vector3 pos, Ogre::Vector3 dir, Ogre::Quaternion rot) {
		if (timeRemainingBeforeShot > fireRate) {
			Weapon_Shot_Space::Weapon_Shot* newShot = new Weapon_Shot_Space::Weapon_Shot (dir, owner_object, weapon_damage);
			newShot->createEntity(man, pos);
			shotsFired.push_back(newShot);
			timeRemainingBeforeShot = 0;
		}
	}

	/*------------------------ Standard Lazer Cannon -------------------*/
	Lazer::Lazer(void) {
		initialize_weapon();
	}

	Lazer::Lazer(Ogre::String owner) {
		initialize_weapon();
		owner_object = owner;
	}

	Lazer::~Lazer(void) {
	}

	void Lazer::initialize_weapon(void) {
		weapon_damage = 1;
		fireRate = 4;
		timeRemainingBeforeShot = 0;
		weapon_name = "Lazer Cannon";
		needsOrientation = false;
		owner_object = "unknown";
	}

	/*------------------------ Bomb Launcher -------------------*/
	Bomb::Bomb(void) {
		initialize_weapon();
	}

	Bomb::Bomb(Ogre::String owner) {
		initialize_weapon();
		owner_object = owner;
	}

	Bomb::~Bomb(void) {
	}

	void Bomb::initialize_weapon(void) {
		weapon_damage = 1000;
		fireRate = 30;
		timeRemainingBeforeShot = 0;
		needsOrientation = false;
		weapon_name = "Bomb Launcher";
		owner_object = "unknown";
	}

	void Bomb::fire_weapon(Ogre::SceneManager* man, Ogre::Vector3 pos, Ogre::Vector3 dir) {
		if (timeRemainingBeforeShot > fireRate) {
			Weapon_Shot_Space::Weapon_Shot* newShot = new Weapon_Shot_Space::Explosive_Shot (dir, owner_object, weapon_damage);
			newShot->createEntity(man, pos);
			shotsFired.push_back(newShot);
			timeRemainingBeforeShot = 0;
		}
	}

	/*------------------------ Scatter Shot Weapon  -------------------*/
	Scatter_Shot::Scatter_Shot(void) {
		initialize_weapon();
	}

	Scatter_Shot::Scatter_Shot(Ogre::String owner) {
		initialize_weapon();
		owner_object = owner;
	}

	Scatter_Shot::~Scatter_Shot(void) {
	}

	void Scatter_Shot::initialize_weapon(void) {
		weapon_damage = 1;
		fireRate = 30;
		timeRemainingBeforeShot = 0;
		needsOrientation = true;
		weapon_name = "Scatter Shooter";
		owner_object = "unknown";
	}

	// Fires The Scatter Shot
	void Scatter_Shot::fire_weapon(Ogre::SceneManager* man, Ogre::Vector3 pos, Ogre::Vector3 dir, Ogre::Quaternion ort) {
		if (timeRemainingBeforeShot > fireRate) {

			for (int i = 0; i < NUM_OF_SCATTER_PELLETS; i++) {

				// Randomly select three numbers to define a point in spherical coordinates
				float radiiPlacement = ((double) rand() / (RAND_MAX)) * 2.0 * Ogre::Math::TWO_PI;
				float percentIn = ((double) rand() / (RAND_MAX));

				// Define the normal and point based on theta, phi and the spray
				Ogre::Vector3 normal = Ogre::Vector3(SCATTER_RADIUS * cos(radiiPlacement) * percentIn, SCATTER_RADIUS * sin (radiiPlacement) * percentIn, dir.normalisedCopy().z);
				normal.normalise();
				normal = ort * normal;

				// Create Our Weapon Shot
				Weapon_Shot_Space::Weapon_Shot* newShot = new Weapon_Shot_Space::Weapon_Shot (normal, owner_object, weapon_damage);
				newShot->createEntity(man, pos);
				shotsFired.push_back(newShot);
			}
			timeRemainingBeforeShot = 0;
		}
	}

}