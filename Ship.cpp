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

	void Ship::createDefaultShip(Ogre::SceneManager* scene_manager, Ogre::Vector3 pos) {
		Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();
		Ogre::Entity *entity;
		Ogre::Matrix4 transformations;
		Ogre::String part_name;
		Ogre::String entity_name = "base_ship" + Ogre::StringConverter::toString(ship_num++);

		entity = scene_manager->createEntity(entity_name, "cube.mesh");
		entity->setMaterialName("ShipTexture");
		ship_node = root_scene_node->createChildSceneNode(entity_name);
		ship_node->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(2.0, 2.0, 2.0));
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(0.0, 0.0, 0.0))) * transformations;
		AssignTransf(ship_node, transformations);

		part_name = "rightarm2";
		entity = scene_manager->createEntity(entity_name + part_name, "cube.mesh");  //mesh name on the right, entity on the left
		entity->setMaterialName("ShipTexture");
		Ogre::SceneNode* rightArm2 = ship_node->createChildSceneNode(entity_name + part_name);
		rightArm2->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(0.5, 0.5, 0.5));
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(-1.25, 0.0, -2.0))) * transformations;
		AssignTransf(rightArm2, transformations);

		part_name = "leftarm2";
		entity = scene_manager->createEntity(entity_name + part_name, "cube.mesh");  //mesh name on the right, entity on the left
		entity->setMaterialName("ShipTexture");
		Ogre::SceneNode* leftArm2 = ship_node->createChildSceneNode(entity_name + part_name);
		leftArm2->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(0.5, 0.5, 0.5));
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(1.25, 0.0, -2.0))) * transformations;
		AssignTransf(leftArm2, transformations);

		part_name = "rightcannon2";
		entity = scene_manager->createEntity(entity_name + part_name, "Cylinder");  //mesh name on the right, entity on the left
		Ogre::SceneNode* rightCannon = ship_node->createChildSceneNode(entity_name + part_name);
		rightCannon->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(0.25, 0.25, 0.25));
		transformations = Ogre::Matrix4(RotationMatrix(Ogre::Vector3(1.0, 0.0, 0.0), Ogre::Radian(Ogre::Math::PI/2.0))) * transformations;
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(-1.25, 0.0, -2.5))) * transformations;
		AssignTransf(rightCannon, transformations);

		part_name = "leftcannon2";
		entity = scene_manager->createEntity(entity_name + part_name, "Cylinder");  //mesh name on the right, entity on the left
		Ogre::SceneNode* leftCannon = ship_node->createChildSceneNode(entity_name + part_name);
		leftCannon->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(0.25, 0.25, 0.25));
		transformations = Ogre::Matrix4(RotationMatrix(Ogre::Vector3(1.0, 0.0, 0.0), Ogre::Radian(Ogre::Math::PI/2.0))) * transformations;
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(1.25, 0.0, -2.5))) * transformations;
		AssignTransf(leftCannon, transformations);

		part_name = "dashboard";
		entity = scene_manager->createEntity(entity_name + part_name, "cube.mesh");
		entity->setMaterialName("DashBoard");
		Ogre::SceneNode* dashboard = ship_node->createChildSceneNode(entity_name + part_name);
		dashboard->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(2.0, 0.35, 0.05));
		transformations = Ogre::Matrix4(RotationMatrix(Ogre::Vector3(-1.0, 0.0, 0.0), Ogre::Radian(Ogre::Math::PI/4))) * transformations;
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(0.0, -0.65, -2.0))) * transformations;
		AssignTransf(dashboard, transformations);

		part_name = "windowParamright";
		entity = scene_manager->createEntity(entity_name + part_name, "cube.mesh");
		entity->setMaterialName("ShipTexture");
		Ogre::SceneNode* windowParamright = ship_node->createChildSceneNode(entity_name + part_name);
		windowParamright->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(0.10, 1.5, 0.10));
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(1.1, 0.0, -2.0))) * transformations;
		AssignTransf(windowParamright, transformations);

		part_name = "windowParamleft";
		entity = scene_manager->createEntity(entity_name + part_name, "cube.mesh");
		entity->setMaterialName("ShipTexture");
		Ogre::SceneNode* windowParamleft = ship_node->createChildSceneNode(entity_name + part_name);
		windowParamleft->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(0.10, 1.5, 0.10));
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(-1.1, 0.0, -2.0))) * transformations;
		AssignTransf(windowParamleft, transformations);

		part_name = "windowParamtop";
		entity = scene_manager->createEntity(entity_name + part_name, "cube.mesh");
		entity->setMaterialName("ShipTexture");
		Ogre::SceneNode* windowParamtop = ship_node->createChildSceneNode(entity_name + part_name);
		windowParamtop->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(1.5, 0.10, 0.10));
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(0.0, 0.65, -1.5))) * transformations;
		AssignTransf(windowParamtop, transformations);
		
		part_name = "windowtopright";
		entity = scene_manager->createEntity(entity_name + part_name, "cube.mesh");
		entity->setMaterialName("ShipTexture");
		Ogre::SceneNode* windowtopright = ship_node->createChildSceneNode(entity_name + part_name);
		windowtopright->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(0.2, 0.3, 0.10));
		transformations = Ogre::Matrix4(RotationMatrix(Ogre::Vector3(0.0, 0.0, 1.0), Ogre::Radian(Ogre::Math::PI/4))) * transformations;
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(0.85, 0.6, -1.5))) * transformations;
		AssignTransf(windowtopright, transformations);

		part_name = "windowtopleft";
		entity = scene_manager->createEntity(entity_name + part_name, "cube.mesh");
		entity->setMaterialName("ShipTexture");
		Ogre::SceneNode* windowtopleft = ship_node->createChildSceneNode(entity_name + part_name);
		windowtopleft->attachObject(entity);

		transformations = Ogre::Matrix4::IDENTITY;
		transformations = transformations * ScalingMatrix(Ogre::Vector3(0.2, 0.3, 0.10));
		transformations = Ogre::Matrix4(RotationMatrix(Ogre::Vector3(0.0, 0.0, -1.0), Ogre::Radian(Ogre::Math::PI/4))) * transformations;
		transformations = Ogre::Matrix4(TranslationMatrix(Ogre::Vector3(-0.85, 0.6, -1.5))) * transformations;
		AssignTransf(windowtopleft, transformations);

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