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
	Weapon_Space::Weapon_Shot* Ship::getCurrentShot(void) {
		return shot;
	}
}