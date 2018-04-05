#pragma once

#include <iostream>
#include "PhysicsObject.h"

class RigidBody : public PhysicsObject
{
public:
	RigidBody();
	RigidBody(glm::vec3 a_position, glm::vec3 a_velocity, glm::quat a_rotation, float a_mass);
	glm::vec3 position;
	glm::vec3 velocity;
	glm::quat rotation;
	glm::vec4 colour;
	vec3 acceleration;
	vec3 _force;
	float mass;
	float rotation2D; //2D so we only need a single float to represent our rotation
	
	void update(glm::vec3 gravity, float timeStep) override;
	//void debug() override;
	void applyForce(glm::vec3 force);
	void applyForceToActor(RigidBody* actor2, glm::vec3 force);
};