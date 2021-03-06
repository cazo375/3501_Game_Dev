#include "CollisionDetector.h"

namespace Collision_Manager {

	// Runs Our Collision Detection For Ray Tracing With Sphere
	bool CollisionManager::runRaySphereCollision(Ogre::Vector3 ray_start, Ogre::Vector3 direction, Ogre::Vector3 sphereCenter, float r){
		Ogre::Vector3 vectorToSphere = ray_start - sphereCenter;
		float dotOfValue = vectorToSphere.dotProduct(direction);
		float lengthOfVecToSphere = vectorToSphere.dotProduct(vectorToSphere) - r * r;

		// If The Dot Value Is Neg, Vector Point Away From Sphere, So Can Check If Ray Start Exists Inside Or Not
		if (dotOfValue > 0.0f && lengthOfVecToSphere > 0.0f) {
			return false;
		}

		// Find Our Discriminate
		float discr = dotOfValue * dotOfValue - lengthOfVecToSphere;

		// If Our Discriminate Is Positive There Is A Collision
		return (discr >= 0.0f);
	};

	bool CollisionManager::runBoundingSphereCollision(Ogre::Vector3 obj1, Ogre::Vector3 obj2, float radius1, float radius2) {
		float combinedRadius = radius1 + radius2;
		Ogre::Vector3 realPos = obj1 - obj2;
		float distanceBetween = realPos.x * realPos.x + realPos.y * realPos.y + realPos.z * realPos.z;
		return distanceBetween <= combinedRadius * combinedRadius;
	}

}