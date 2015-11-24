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

	// Fires The Weapon When Called
	void BaseWeapon::fire_weapon(Ogre::SceneManager* man, Ogre::Vector3 pos, Ogre::Vector3 dir) {
		if (timeRemainingBeforeShot > fireRate) {
			Weapon_Shot_Space::Weapon_Shot* newShot = new Weapon_Shot_Space::Weapon_Shot (dir, owner_object, weapon_damage);
			newShot->createEntity(man, pos);
			shotsFired.push_back(newShot);
			timeRemainingBeforeShot = 0;
		}
	}

	/*------------------------ Standard Lazer Cannon -------------------*/
	Lazer::Lazer(void) {
		weapon_damage = 1;
		fireRate = 4;
		timeRemainingBeforeShot = 0;
		weapon_name = "Lazer Cannon";
		owner_object = "unknown";
	}

	Lazer::Lazer(Ogre::String owner) {
		weapon_damage = 1;
		fireRate = 4;
		timeRemainingBeforeShot = 0;
		weapon_name = "Lazer Cannon";
		owner_object = owner;
	}

	Lazer::~Lazer(void) {
	}


	/*------------------------ Bomb Launcher -------------------*/
	Bomb::Bomb(void) {
		weapon_damage = 1000;
		fireRate = 30;
		timeRemainingBeforeShot = 0;

		weapon_name = "Bomb Launcher";
		owner_object = "unknown";
	}

	Bomb::Bomb(Ogre::String owner) {
		weapon_damage = 1000;
		fireRate = 30;
		timeRemainingBeforeShot = 0;

		weapon_name = "Bomb Launcher";
		owner_object = owner;
	}

	Bomb::~Bomb(void) {
	}

	void Bomb::fire_weapon(Ogre::SceneManager* man, Ogre::Vector3 pos, Ogre::Vector3 dir) {
		if (timeRemainingBeforeShot > fireRate) {
			Weapon_Shot_Space::Weapon_Shot* newShot = new Weapon_Shot_Space::Explosive_Shot (dir, owner_object, weapon_damage);
			newShot->createEntity(man, pos);
			shotsFired.push_back(newShot);
			timeRemainingBeforeShot = 0;
		}
	}
}