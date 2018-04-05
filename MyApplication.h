#include "FlyCamera.h"
#include "ParticleEmitter.h"
#include "LightingApp.h"
#include "Textures.h"
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include "FBXFile.h"
#include "BoundingSphere.h"

#define GLM_SWIZZLE

#include <gl_core_4_4.h>
#include <GLFW\glfw3.h>

#include <Gizmos.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "PhysicsScene.h"

#include "SpringJoint.h"


using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;


#pragma once
class MyApplication
{
public:

	int Run();

	unsigned int m_VAO;	//vertex array object
	unsigned int m_VBO; //vertex buffer object
	unsigned int m_IBO;	//index buffer object

	void setCamera();

	virtual bool createWindow(const char* title, int width, int height);

	float deltaTime;
	float previousTime;

protected:
	/*virtual*/ bool Init();
	/*virtual*/ bool Update();
	/*virtual*/ void Draw(const mat4& m_projectionViewMatrix);
	/*virtual*/ void Destroy();

	int m_uiScreenWidth = 1080;
	int m_uiScreenHeight = 720;
	vec3 m_clearColour;
	float snowLevel;

	unsigned int m_programID;
	//void generateGrid(unsigned int rows, unsigned int cols);
	unsigned int indexCount;
	
	FlyCamera* fCam;
	GLFWwindow*	m_window;
	ParticleEmitter* m_particles;
	LightingApp* m_light;
	LightingApp* m_light2;
	Textures* m_texture;
	BoundingSphere* boundingSphere;

	FBXFile* soulSpear;
	FBXFile* dragon;

	SpringJoint* spring;
	PhysicsScene* physicsScene;
	virtual void destroyWindow();


	SphereClass* ball;
	SphereClass* ball1;
	SphereClass* ball2;
	SphereClass* ball3;

	PlaneClass* plane;

	BoxClass* staticBox;
	BoxClass* aquabox;
	BoxClass* moxbox;
	SphereClass* sphere;

	BoxClass* testBox;
	SphereClass* testSphere;

};

struct Vertex
{
	vec4 position;
	vec4 colour;
	vec2 texCoords;
};