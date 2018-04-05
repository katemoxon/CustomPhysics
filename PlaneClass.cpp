#include "PlaneClass.h"



PlaneClass::PlaneClass(glm::vec3 normal, float distance)
{
	this->normal = normal;

	this->distance = distance;

	_shapeID = PLANE;
}







void PlaneClass::makeGizmo()
{
	//float lineSegmentLength = 300;
	glm::vec3 centrePoint = normal * distance;
	//glm::vec3 parallel = glm::vec3(normal.y, -normal.x); //easy to rotate normal through 90degrees around z
	//glm::vec4 colour(1, 1, 1, 1);
	//glm::vec3 start = centrePoint + (parallel * lineSegmentLength);
	//glm::vec3 end = centrePoint - (parallel * lineSegmentLength);
	//Gizmos::add2DLine(start.xy(), end.xy(), colour);

	Gizmos::addAABBFilled(centrePoint, vec3(100, 0.1f, 100), vec4(0.25f, 0.25f, 0.25f, 1));
}