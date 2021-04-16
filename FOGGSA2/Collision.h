#pragma once
#ifndef COLLISION_H
#define COLLISION_H

class SceneObject;

namespace Collision{
	bool Sphere(SceneObject* obj1, SceneObject* obj2);
}

#endif