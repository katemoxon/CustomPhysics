#pragma once

#include <gl_core_4_4.h>
#include <GLFW\glfw3.h>
#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"


#include <Gizmos.h>
#include <vector>
#include <algorithm>

#include "PhysicsObject.h"
#include "RigidBody.h"
#include "SphereClass.h"
#include "PlaneClass.h"
#include "BoxClass.h"

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;

class PhysicsScene
{
public:
	PhysicsScene();
	~PhysicsScene();
	glm::vec3 gravity;
	float timeStep;
	std::vector<PhysicsObject*> actors;
	void addActor(PhysicsObject*);
	void removeActor(PhysicsObject*);
	void update();
	void addGizmos();

	void checkForCollision();

	static void plane2Plane(PhysicsObject*, PhysicsObject*);
	static void plane2Sphere(PhysicsObject*, PhysicsObject*);
	static void plane2Box(PhysicsObject*, PhysicsObject*);

	static void sphere2Plane(PhysicsObject*, PhysicsObject*);
	static void sphere2Sphere(PhysicsObject*, PhysicsObject*);
	static void sphere2Box(PhysicsObject*, PhysicsObject*);

	static void box2Plane(PhysicsObject*, PhysicsObject*);
	static void box2Sphere(PhysicsObject*, PhysicsObject*);
	static void box2Box(PhysicsObject*, PhysicsObject*);

	static void collisionSeparate(PhysicsObject* obj1, PhysicsObject* obj2, vec3 normal, float overlap);
	static void collisionResolution(PhysicsObject* obj1, PhysicsObject* obj2, vec3 normal, float overlap);
	static void Dynamic2Dynamic(PhysicsObject* obj1, PhysicsObject* obj2, vec3 normal);
};
