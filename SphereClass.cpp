#include "SphereClass.h"



SphereClass::SphereClass(glm::vec3 position,
						 glm::vec3 velocity, float mass, float radius, bool Kinematic,
						 glm::vec4 colour)
{
	this->position = position;
	this->velocity = velocity;
	this->mass = mass;
	_radius = radius;
	isKinematic = Kinematic;
	this->colour = colour;
	_shapeID = SPHERE;
}

SphereClass::~SphereClass() {}

void SphereClass::makeGizmo()
{
	Gizmos::addSphere(position, _radius, 24, 24, colour);
}