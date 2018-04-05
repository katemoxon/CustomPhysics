#include <gl_core_4_4.h>
#include <GLFW\glfw3.h>
#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"


#include <Gizmos.h>


using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;


#pragma once
class SolarSystem
{
public:
	SolarSystem();
	~SolarSystem();

	void Init();
	int Update();
	void Draw();
	void Destroy();

	void Comets();
};