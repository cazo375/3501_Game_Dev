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
		removeAllShots();
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

	void BaseWeapon::removeAllShots(void) {
		for (int i = 0; i < shotsFired.size(); i++) {
			delete shotsFired[i];
		}

		shotsFired.clear();
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

	void BaseWeapon::fire_weapon (Ogre::SceneManager* man, Ogre::Vector3 pos, Ogre::Vector3 dir, Ogre::Vector3 upDir) {
		fire_weapon (man, pos, dir);
	}

	void BaseWeapon::fire_weapon(Ogre::SceneManager* man, Ogre::Vector3 pos, Ogre::Vector3 dir, Ogre::Quaternion rot) {
		if (timeRemainingBeforeShot > fireRate) {
			Weapon_Shot_Space::Weapon_Shot* newShot = new Weapon_Shot_Space::Weapon_Shot (dir, owner_object, weapon_damage);
			newShot->createEntity(man, pos);
			shotsFired.push_back(newShot);
			timeRemainingBeforeShot = 0;
		}
	}

	/*-------------------------------------------------------- Standard Laser Cannon ----------------------------------------------------*/
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

	/*-------------------------------------------------------- Enemy Based Laser Cannon ----------------------------------------------------*/
	Enemy_Lazer_Cannon::Enemy_Lazer_Cannon(void) {
		initialize_weapon();
	}

	Enemy_Lazer_Cannon::Enemy_Lazer_Cannon(Ogre::String owner) {
		initialize_weapon();
		owner_object = owner;
	}

	Enemy_Lazer_Cannon::~Enemy_Lazer_Cannon(void) {
	}

	void Enemy_Lazer_Cannon::initialize_weapon(void) {
		weapon_damage = 1;
		fireRate = 20.0f;
		timeRemainingBeforeShot = 0;
		weapon_name = "Lazer Cannon";
		needsOrientation = false;
		owner_object = "unknown";
	}

	/*-------------------------------------------------------- Bomb Launcher ----------------------------------------------------*/
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


	/*-------------------------------------------------------- Spline Launcher Base ----------------------------------------------------*/
	Spline_Bomb_Launcher::Spline_Bomb_Launcher(void) {
		initialize_weapon();
	}

	Spline_Bomb_Launcher::Spline_Bomb_Launcher(Ogre::String owner) {
		initialize_weapon();
		owner_object = owner;
	}

	Spline_Bomb_Launcher::~Spline_Bomb_Launcher(void) {
	}

	void Spline_Bomb_Launcher::initialize_weapon(void) {
		weapon_damage = 12;
		fireRate = 60;
		timeRemainingBeforeShot = 0;
		needsOrientation = false;
		weapon_name = "Spline Bomb Launcher";
		owner_object = "unknown";
	}

	void Spline_Bomb_Launcher::fire_weapon (Ogre::SceneManager* man, Ogre::Vector3 pos, Ogre::Vector3 dir, Ogre::Vector3 upDir) {
		if (timeRemainingBeforeShot > fireRate) {
			Ogre::Vector3 upVector = upDir;
			Ogre::Vector3 crossProduct = pos.crossProduct(upVector).normalisedCopy();

			create_splinic_shot (man, pos, crossProduct, dir);
			create_splinic_shot (man, pos, -crossProduct, dir);
			create_splinic_shot (man, pos, upVector, dir);
			create_splinic_shot (man, pos, -upVector, dir);

			timeRemainingBeforeShot = 0;
		}
	}

	void Spline_Bomb_Launcher::create_splinic_shot(Ogre::SceneManager* man, Ogre::Vector3 pos, Ogre::Vector3 spline_dir, Ogre::Vector3 dir) {
		Weapon_Shot_Space::Weapon_Shot* newShot = new Weapon_Shot_Space::Splinic_Shot (pos, spline_dir, dir, owner_object, weapon_damage);
		newShot->createEntity(man, pos);
		shotsFired.push_back(newShot);
	}

	/*-------------------------------------------------------- Scatter Shot Base ----------------------------------------------------*/
	Ogre::Vector3 Scatter_Base:: create_spray_dir (Ogre::Vector3 originalDir) {
		// Randomly select three numbers to define a point in spherical coordinates
		float xDisplacement = (-0.5 + ((double) rand() / (RAND_MAX))) * scatter_range;
		float yDisplacement = (-0.5 + ((double) rand() / (RAND_MAX))) * scatter_range;

		// Define the normal and point based on theta, phi and the spray
		Ogre::Vector3 baseDir = originalDir.normalisedCopy();
		Ogre::Vector3 newDir = Ogre::Vector3(baseDir.x + xDisplacement, baseDir.y + yDisplacement, baseDir.z);
		newDir.normalise();

		return newDir;
	}

	/*------------------------------------------------------- Scatter Shot Weapon  ---------------------------------------------------*/
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
		scatter_range = 0.1f;
		needsOrientation = true;
		weapon_name = "Scatter Shooter";
		owner_object = "unknown";
	}

	// Fires The Scatter Shot
	void Scatter_Shot::fire_weapon(Ogre::SceneManager* man, Ogre::Vector3 pos, Ogre::Vector3 dir, Ogre::Quaternion ort) {
		if (timeRemainingBeforeShot > fireRate) {

			for (int i = 0; i < NUM_OF_SCATTER_PELLETS; i++) {
				Ogre::Vector3 newDir = create_spray_dir(dir);

				// Create Our Weapon Shot
				Weapon_Shot_Space::Weapon_Shot* newShot = new Weapon_Shot_Space::Weapon_Shot (newDir, owner_object, weapon_damage);
				newShot->createEntity(man, pos);
				shotsFired.push_back(newShot);
			}
			timeRemainingBeforeShot = 0;
		}
	}


	/*------------------------ Scatter Bomb Shot Weapon  -------------------*/
	Scatter_Bomb_Shot::Scatter_Bomb_Shot(void) {
		initialize_weapon();
	}

	Scatter_Bomb_Shot::Scatter_Bomb_Shot(Ogre::String owner) {
		initialize_weapon();
		owner_object = owner;
	}

	Scatter_Bomb_Shot::~Scatter_Bomb_Shot(void) {
	}

	void Scatter_Bomb_Shot::initialize_weapon(void) {
		weapon_damage = 10;
		fireRate = 70;
		timeRemainingBeforeShot = 0;
		scatter_range = 0.5f;
		needsOrientation = true;
		weapon_name = "Scatter Bomb Launcher";
		owner_object = "unknown";
	}

	// Fires The Scatter Shot
	void Scatter_Bomb_Shot::fire_weapon(Ogre::SceneManager* man, Ogre::Vector3 pos, Ogre::Vector3 dir, Ogre::Quaternion ort) {
		if (timeRemainingBeforeShot > fireRate) {

			for (int i = 0; i < NUM_OF_SCATTER_PELLETS; i++) {
				Ogre::Vector3 newDir = create_spray_dir(dir);

				// Create Our Weapon Shot
				Weapon_Shot_Space::Weapon_Shot* newShot = new Weapon_Shot_Space::Explosive_Shot (newDir, owner_object, weapon_damage);
				newShot->createEntity(man, pos);
				shotsFired.push_back(newShot);
			}
			timeRemainingBeforeShot = 0;
		}
	}

}