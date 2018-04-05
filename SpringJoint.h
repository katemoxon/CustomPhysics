#pragma once
#include "PhysicsObject.h"
#include "RigidBody.h"

class SpringJoint : public PhysicsObject
{
public:
	SpringJoint(RigidBody* connection1, RigidBody* connection2,
		float springCoefficient, float damping);
	~SpringJoint();

private:
	void update(vec3 gravity, float timeStep) override;
	//void debug() override;
	void makeGizmo() override;

	RigidBody* _connections[2];
	float _damping;
	float _restLength;
	float _springCoefficient;
};