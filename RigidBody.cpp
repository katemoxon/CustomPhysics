#include "RigidBody.h"



RigidBody::RigidBody() {};

RigidBody::RigidBody(glm::vec3 a_position, glm::vec3 a_velocity, glm::quat a_rotation, float a_mass)
{
	this->position = a_position;
	this->velocity = a_velocity;
	this->rotation = a_rotation;
	this->mass = a_mass;
}




void RigidBody::update(glm::vec3 gravity, float timeStep)
{
	//if (isKinematic)
	//{
	//
	//	applyForce(gravity * mass * timeStep);
	//
	//	position += velocity * timeStep;
	//}

	if (!isKinematic)
	{
		velocity *= 0.9;
		acceleration = gravity;
		acceleration += _force;
		velocity += acceleration * timeStep;
		position += velocity * timeStep;
		_force = vec3(0);
	}
}

//void RigidBody::debug()
//{
//	std::cout << "Position: " << position.x << "," << position.y << /*"," << position.z <<*/ std::endl;
//}

void RigidBody::applyForce(glm::vec3 force)
{
	_force += force / mass;
}

void RigidBody::applyForceToActor(RigidBody* actor2, glm::vec3 force)
{
	actor2->applyForce(force);
	applyForce(-force);
}