#pragma once

#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>
#define GLM_SWIZZLE
#include <Gizmos.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

using glm::vec3;
using glm::vec4;
using glm::mat4;

struct Vertex
{
	Vertex();
	~Vertex();

	vec4 position;
	vec4 colour;
	
	void generateGrid(unsigned int rows, unsigned int cols);
};

