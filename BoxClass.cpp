#include "BoxClass.h"



BoxClass::BoxClass(vec3 position, vec3 velocity, bool Kinematic, float mass,
	vec3 scale, vec4 colour)
{
	_shapeID = BOX;
	this->position = position;
	this->velocity = velocity;
	this->mass = mass;
	m_scale = scale;
	this->colour = colour;
	isKinematic = Kinematic;


	//if (isKinematic)
	//{
	//	m_mass = INFINITY;
	//	m_velocity = vec3(0);
	//}
}

void BoxClass::makeGizmo()
{
	Gizmos::addAABBFilled(position, m_scale, colour);
}