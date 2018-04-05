#include "SolarSystem.h"

SolarSystem::SolarSystem(){}

SolarSystem::~SolarSystem(){}

void SolarSystem::Init(){}

int SolarSystem::Update()
{
	if (glfwInit() == false)
	{
		return -1;
	}
	GLFWwindow* window = glfwCreateWindow(1280, 720,
		"Computer Graphics",
		nullptr, nullptr);

	if (window == nullptr)
	{
		glfwTerminate();
		return -2;
	}

	glfwMakeContextCurrent(window);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(window);
		glfwTerminate();
		return -3;
	}

	auto major = ogl_GetMajorVersion();
	auto minor = ogl_GetMinorVersion();
	printf("GL: %i.%i\n", major, minor);

	Gizmos::create();

	mat4 view = glm::lookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
	mat4 projection = glm::perspective(glm::pi<float>() * 0.25f,
		16 / 9.f, 0.1f, 1000.f);


	glm::mat4 sunRot;
	glm::mat4 planetRot;

	planetRot = glm::translate(planetRot, vec3(5, 0, 5));

	glm::mat4 planetOrbit;

	glm::mat4 comet1path;
	comet1path = glm::translate(comet1path, vec3(5, 0, 0));


	while (glfwWindowShouldClose(window) == false &&
		glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{

		glClearColor(0.25f, 0.25f, 0.25f, 1);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Gizmos::clear();

		Gizmos::addTransform(glm::mat4(1));

		vec4 white(1);

		vec4 black(0, 0, 0, 1);

		for (int i = 0; i < 21; ++i)
		{
			Gizmos::addLine(vec3(-10 + i, 0, 10),
				vec3(-10 + i, 0, -10),
				i == 10 ? white : black);

			Gizmos::addLine(vec3(10, 0, -10 + i),
				vec3(-10, 0, -10 + i),
				i == 10 ? white : black);


		}

		sunRot = glm::rotate(sunRot, 0.001f, vec3(0, 1, 0));
		Gizmos::addSphere(vec3(0, 0, 0), 2, 50, 50, vec4(0, 1, 1, 1), &sunRot);

		planetRot = glm::rotate(planetRot, -0.005f, vec3(0, 1, 0));
		planetOrbit = sunRot * planetRot;

		Gizmos::addSphere(planetOrbit[3].xyz, 0.75f, 15, 15, vec4(0, 0, 1, 1), &planetOrbit);

		
		//comet1path = glm::translate(comet1path, vec3(5, 0, 0)); //movement
		Gizmos::addSphere(comet1path[3].xyz, 0.75f, 15, 15, vec4(1.5, 1.5, 1.5, 0), &comet1path);


		//Gizmos::addSphere(vec3(0, 0, 5), 0.75f, 15, 15, vec4(0, 0, 1, 1), &planetRot);

		//Gizmos::draw(fCam->GetProjectionView());
		Gizmos::draw(projection * view);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	Gizmos::destroy();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void SolarSystem::Draw(){}

void SolarSystem::Destroy(){}


void SolarSystem::Comets()
{


}