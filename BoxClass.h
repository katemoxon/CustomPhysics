#pragma once
#include "RigidBody.h"

class BoxClass : public RigidBody
{
public:
	BoxClass() {};
	BoxClass(vec3 position, vec3 velocity, bool Kinematic, float mass, vec3 scale, vec4 colour);
	~BoxClass() {}

	vec3 m_scale;

	vec3 min;
	vec3 max;
	void makeGizmo();
};