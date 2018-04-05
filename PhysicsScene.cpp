#include "PhysicsScene.h"



PhysicsScene::PhysicsScene()
{
}


PhysicsScene::~PhysicsScene()
{
}

void PhysicsScene::addActor(PhysicsObject* object)
{
	//auto item = std::find(actors.begin(), actors.end(), object);
	//if (item < actors.end())
	//{
	//	actors.push_back(object);
	//}

	actors.push_back(object);
}

void PhysicsScene::removeActor(PhysicsObject* object)
{
	auto item = std::find(actors.begin(), actors.end(), object);
	if (item < actors.end())
	{
		actors.erase(item);
	}
}

void PhysicsScene::update()
{
	for (auto actorPtr : actors)
	{
		actorPtr->update(gravity, timeStep);
	}

	checkForCollision();
}

void PhysicsScene::addGizmos()
{
	for (int i = 0; i < actors.size(); i++)
	{
		actors[i]->makeGizmo();
	}
}

void PhysicsScene::plane2Plane(PhysicsObject* object1, PhysicsObject* object2)
{
	return;
}

void PhysicsScene::plane2Sphere(PhysicsObject* object1, PhysicsObject* object2)
{
	sphere2Plane(object2, object1);
}

void PhysicsScene::plane2Box(PhysicsObject* object1, PhysicsObject* object2)
{
	box2Plane(object2, object1);
}

void PhysicsScene::sphere2Plane(PhysicsObject* object1, PhysicsObject* object2)
{
	SphereClass* sphere = dynamic_cast<SphereClass*>(object1);
	PlaneClass* plane = dynamic_cast<PlaneClass*>(object2);

	if (plane != NULL && sphere != NULL)
	{
		glm::vec3 collisionNormal = plane->normal;
		float sphereToPlane = glm::dot(sphere->position, plane->normal) - plane->distance;
		if (sphereToPlane < 0) //planes are infintely thin, double sided, objects so if we are behind it we flip the normal
		{
			collisionNormal *= -1;
			sphereToPlane *= -1;
		}
		float intersection = sphere->_radius - sphereToPlane; //intersection between sphere and plane
		if (intersection > 0)
		{
			collisionResolution(sphere, plane, collisionNormal, intersection);
		}
	}
}

void PhysicsScene::sphere2Sphere(PhysicsObject* object1, PhysicsObject* object2)
{
	SphereClass* sphere1 = dynamic_cast<SphereClass*>(object1);
	SphereClass* sphere2 = dynamic_cast<SphereClass*>(object2);

	if (sphere1 != NULL && sphere2 != NULL)
	{
		vec3 delta = sphere2->position - sphere1->position;
		vec3 collisionNormal = glm::normalize(delta);
		float distance = glm::length(delta);
		float intersection = sphere1->_radius + sphere2->_radius - distance;
		if (intersection > 0)
		{
			collisionResolution(sphere1, sphere2, collisionNormal, intersection);
		}
	}
}

void PhysicsScene::sphere2Box(PhysicsObject* object1, PhysicsObject* object2)
{
	SphereClass* sphere = dynamic_cast<SphereClass*>(object1);
	BoxClass* box = dynamic_cast<BoxClass*>(object2);

	if (sphere != NULL && box != NULL)
	{
		box->min = box->position - box->m_scale;
		box->max = box->position + box->m_scale;

		float x = glm::max(box->min.x, glm::min(sphere->position.x, box->max.x));
		float y = glm::max(box->min.y, glm::min(sphere->position.y, box->max.y));
		float z = glm::max(box->min.z, glm::min(sphere->position.z, box->max.z));

		vec3 point(x, y, z);

		vec3 offset = sphere->position - point;
		vec3 collisionNormal = glm::normalize(offset);
		float distance = glm::length(offset);
		float overlap = sphere->_radius - distance;

		if (overlap > 0)
		{
			collisionResolution(sphere, box, collisionNormal, -overlap);
		}
	}
}

void PhysicsScene::box2Plane(PhysicsObject* object1, PhysicsObject* object2)
{
	BoxClass* box = dynamic_cast<BoxClass*>(object1);
	PlaneClass* plane = dynamic_cast<PlaneClass*>(object2);

	if (box != NULL & plane != NULL)
	{
		box->min = vec3(box->position.x - (box->m_scale.x / 2), box->position.y - (box->m_scale.y / 2), box->position.z - (box->m_scale.z / 2));
		box->max = vec3(box->position.x + (box->m_scale.x / 2), box->position.y + (box->m_scale.y / 2), box->position.z + (box->m_scale.z / 2));

		glm::vec3 collisionNormal = plane->normal;
		float boxToPlane = glm::dot(box->position, plane->normal) - plane->distance;

		if (boxToPlane < 0)
		{
			collisionNormal *= -1;
			boxToPlane *= -1;
		}

		float half = box->max.y - box->min.y;
		float intersection = half - boxToPlane;
		if (intersection > 0)
		{
			collisionResolution(box, plane, collisionNormal, intersection);
		}
	}
}

void PhysicsScene::box2Sphere(PhysicsObject* object1, PhysicsObject* object2)
{
	sphere2Box(object2, object1);
}

void PhysicsScene::box2Box(PhysicsObject* object1, PhysicsObject* object2)
{
	BoxClass* box1 = dynamic_cast<BoxClass*>(object1);
	BoxClass* box2 = dynamic_cast<BoxClass*>(object2);

	if (box1 != NULL && box2 != NULL)
	{
		box1->min = vec3(box1->position.x - box1->m_scale.x, box1->position.y - box1->m_scale.y, box1->position.z - box1->m_scale.z);
		box1->max = vec3(box1->position.x + box1->m_scale.x, box1->position.y + box1->m_scale.y, box1->position.z + box1->m_scale.z);

		box2->min = vec3(box2->position.x - box2->m_scale.x, box2->position.y - box2->m_scale.y, box2->position.z - box2->m_scale.z);
		box2->max = vec3(box2->position.x + box2->m_scale.x, box2->position.y + box2->m_scale.y, box2->position.z + box2->m_scale.z);
		
		if (box1->min.x < box2->max.x &&
			box1->max.x > box2->min.x &&
			box1->min.y < box2->max.y &&
			box1->max.y > box2->min.y &&
			box1->min.z < box2->max.z &&
			box1->max.z > box2->min.z)
		{
			vec3 minOffset = box1->max - box2->min;
			vec3 maxOffset = box1->min - box2->max;

			glm::vec3 offset = glm::vec3(
				std::abs(minOffset.x) < std::abs(maxOffset.x) ? minOffset.x : maxOffset.x,
				std::abs(minOffset.y) < std::abs(maxOffset.y) ? minOffset.y : maxOffset.y,
				std::abs(minOffset.z) < std::abs(maxOffset.z) ? minOffset.z : maxOffset.z);

			offset.x = std::abs(offset.x) < std::abs(offset.y) ? offset.x : 0.0f;
			offset.y = std::abs(offset.y) < std::abs(offset.z) ? offset.y : 0.0f;
			offset.z = std::abs(offset.z) < std::abs(offset.x) ? offset.x : 0.0f;

			vec3 collisionNormal = glm::normalize(offset);
			float interceptDistance = glm::length(offset);

			if (interceptDistance > 0)
			{
				collisionResolution(box1, box2, collisionNormal, interceptDistance * 2);
			}
		}
	}
}

//function pointer array for doing our collisions
typedef void(*fn)(PhysicsObject*, PhysicsObject*);
//function pointer array for doing our collisions
static fn collisionFunctionArray[] =
{
	PhysicsScene::plane2Plane,	PhysicsScene::plane2Sphere,		PhysicsScene::plane2Box,
	PhysicsScene::sphere2Plane,	PhysicsScene::sphere2Sphere,	PhysicsScene::sphere2Box,
	PhysicsScene::box2Plane,	PhysicsScene::box2Sphere,		PhysicsScene::box2Box
};

void PhysicsScene::checkForCollision()
{
	int actorCount = actors.size();
	//need to check for collisions against all objects except this one.
	for (int outer = 0; outer < actorCount - 1; outer++)
	{
		for (int inner = outer + 1; inner < actorCount; inner++)
		{
			PhysicsObject* object1 = actors[outer];
			PhysicsObject* object2 = actors[inner];
			int _shapeID1 = object1->_shapeID;
			int _shapeID2 = object2->_shapeID;
			if (_shapeID1 >= NUMBERSHAPE || _shapeID2 >= NUMBERSHAPE) continue;

			//using function pointers
			int functionIndex = (_shapeID1 * NUMBERSHAPE) + _shapeID2;
			fn collisionFunctionPtr = collisionFunctionArray[functionIndex];
			if (collisionFunctionPtr != NULL)
			{
				collisionFunctionPtr(object1, object2);
			}
		}
	}
}

void PhysicsScene::collisionSeparate(PhysicsObject* object1, PhysicsObject* object2, vec3 normal, float overlap)
{
	RigidBody* obj1 = dynamic_cast<RigidBody*>(object1);
	RigidBody* obj2 = dynamic_cast<RigidBody*>(object2);

	if (obj1 && obj2)
	{
		float totalMass = obj1->mass + obj2->mass;

		float obj1ratio = obj1->mass / totalMass;
		float obj2ratio = obj2->mass / totalMass;

		if (object1->isKinematic)
		{
			obj1ratio = 1;
			obj2ratio = 0;
		}
		if (object2->isKinematic)
		{
			obj2ratio = 1;
			obj1ratio = 0;
		}

		vec3 separation = normal * overlap;

		obj1->position -= separation * obj2ratio;
		obj2->position += separation * obj1ratio;
	}

	else if (obj1 && !obj1->isKinematic)
	{
		vec3 separation = normal * overlap;
		obj1->position += separation;
	}

	else if (obj2 && !obj2->isKinematic)
	{
		vec3 separation = normal * overlap;
		obj2->position -= separation;
	}
}

void PhysicsScene::collisionResolution(PhysicsObject* object1, PhysicsObject* object2, vec3 normal, float overlap)
{
	collisionSeparate(object1, object2, normal, overlap);
	Dynamic2Dynamic(object1, object2, normal);
}

void PhysicsScene::Dynamic2Dynamic(PhysicsObject* obj1, PhysicsObject* obj2, vec3 normal)
{
	RigidBody* object1 = dynamic_cast<RigidBody*>(obj1);
	RigidBody* object2 = dynamic_cast<RigidBody*>(obj2);

	vec3 relativeVelocity = vec3(0);
	float jBottom = 0;

	if (object1 && !object1->isKinematic)
	{
		jBottom += 1 / object1->mass;
		relativeVelocity = object1->velocity;
	}

	if (object2 && !object2->isKinematic)
	{
		jBottom += 1 / object2->mass;
		relativeVelocity -= object2->velocity;
	}

	float e = 0.25f;
	float jTop = glm::dot((-(1 + e) * relativeVelocity), normal);

	float j = jTop / jBottom;

	if (object1 && !object1->isKinematic)
		object1->velocity += (j / object1->mass) * normal;
	if (object2 && !object2->isKinematic)
		object2->velocity -= (j / object2->mass) * normal;
}