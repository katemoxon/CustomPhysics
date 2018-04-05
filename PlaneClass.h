#pragma once
#include "PhysicsObject.h"

class PlaneClass : public PhysicsObject
{
public:
	glm::vec3 normal;
	float distance;
	void virtual update(glm::vec3 gravity, float timeStep) {};
	void virtual debug() {};
	void virtual makeGizmo();
	PlaneClass(glm::vec3 normal, float distance);

};