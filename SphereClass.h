#pragma once
#include "RigidBody.h"

class SphereClass : public RigidBody
{
public:
	SphereClass(glm::vec3 position,
		glm::vec3 velocity, float mass, float radius, bool Kinematic,
		glm::vec4 colour);
	~SphereClass();

	float _radius;
	void makeGizmo();
};
