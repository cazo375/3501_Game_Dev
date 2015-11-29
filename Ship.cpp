#include "Ship.h"

namespace Ship_Space {

	Ogre::Vector3 Ship::getPosition(void) {
		return ship_node->getPosition();
	}

	// Moves The Ship Lazer When Called
	void Ship::moveLazer(void) {
		if (shot) {
			shot->moveShot();

			if (shot->shouldDestoryShot()) {
				delete shot;
				shot = nullptr;
			}
		}
	}

	// Returns The Current Shot
	std::vector<Weapon_Shot_Space::Weapon_Shot*> Ship::getCurrentShots(void) {
		return weapons[currentWeaponIndex]->getShotsFired();
	}

	// Destorys All Weaposn When Called
	void Ship::clearWeapons(void) {
		for (int i = 0; i < weapons.size(); i++) {
			delete weapons[i];
		}

		weapons.empty();
	}
}