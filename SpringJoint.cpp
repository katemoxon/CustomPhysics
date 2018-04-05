#include "SpringJoint.h"



SpringJoint::SpringJoint(RigidBody* connection1, RigidBody* connection2, float
	springCoefficient, float damping)
{
	_connections[0] = connection1;
	_connections[1] = connection2;
	_springCoefficient = springCoefficient;
	_damping = damping;
	_restLength = glm::distance(_connections[0]->position, _connections[1]->position);
	_shapeID = JOINT;
}


SpringJoint::~SpringJoint()
{
}

void SpringJoint::update(vec3 gravity, float timeStep)
{
	float k = _springCoefficient;
	float x = glm::distance(_connections[0]->position, _connections[1]->position) - _restLength;
	float b = _damping;
	vec3 v = _connections[0]->velocity - _connections[0]->velocity * b;
	vec3 f = -k * x - v;

	vec3 force = glm::normalize(_connections[0]->position - _connections[1]->position) * f;

	_connections[1]->applyForceToActor(_connections[0], force);

	//Hooke's law:
	//F = -kX
	//F = force, k = spring coefficient, x = displacement

	//Hooke's law with friction:
	//F = -kX - bv
	//b = damping value, v = velocity

}

void SpringJoint::makeGizmo()
{
	Gizmos::addLine(_connections[0]->position, _connections[1]->position, vec4(1, 1, 1, 1));
}