#include "Ship.h"

namespace Ship_Space {

	static int ship_num = 0;

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

	// Create a rotation matrix based on an angle and an axis
	Ogre::Matrix4 Ship::RotationMatrix(Ogre::Vector3 axis, Ogre::Radian angle){

		Ogre::Matrix3 mat;
		mat = Ogre::Matrix3::IDENTITY;
		mat.FromAngleAxis(axis, angle);
		return Ogre::Matrix4(mat);
	}


	// Create a translation matrix based on a vector of translations (x, y, z)
	Ogre::Matrix4 Ship::TranslationMatrix(Ogre::Vector3 trans){
		Ogre::Matrix4 mat;
		mat = Ogre::Matrix4::IDENTITY;
		mat.setTrans(trans);
		return mat;
	}


	// Create a scaling matrix based on a vector of scale factors (x, y, z)
	Ogre::Matrix4 Ship::ScalingMatrix(Ogre::Vector3 scale){
		Ogre::Matrix4 mat;
		mat = Ogre::Matrix4::IDENTITY;
		mat.setScale(scale);
		return mat;
	}

	void Ship::AssignTransf(Ogre::SceneNode* node, Ogre::Matrix4 transf){

		/* In many graphic frameworks, we would simply multiply our geometry by the transformation matrix.
		However, OGRE stores the transformations of a node in a more efficient manner.
		So, we need to decompose the transformation first into three components and then assign them
		to the scene node.*/
		Ogre::Vector3 trans, scale;
		Ogre::Quaternion quat;

		transf.decomposition(trans, scale, quat);
		node->setScale(scale);
		node->setOrientation(quat);
		node->setPosition(trans);
	}

}