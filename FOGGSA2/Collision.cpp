#include "Collision.h"
#include "SceneObject.h"
namespace Collision {
	bool Sphere(SceneObject* obj1, SceneObject* obj2) {
		Vector3 obj1_pos = obj1->GetPosition();
		Vector3 obj2_pos = obj2->GetPosition();

		// Calculate the squared distance between both objects.
		float distance = ((obj1_pos.x - obj2_pos.x) * (obj1_pos.x - obj2_pos.x));
		distance += ((obj1_pos.y - obj2_pos.y) * (obj1_pos.y - obj2_pos.y));
		distance += ((obj1_pos.z - obj2_pos.z) * (obj1_pos.z - obj2_pos.z));

		// Calculate the minimum distance required for a collision to register.
		float minimum_distance = pow(obj1->GetCollisionRadius() + obj2->GetCollisionRadius(),2);

		// If the distance is smaller than the calculated "minimum" distance, then a spherical collision has occured.
		return (distance <= minimum_distance);
	}
}