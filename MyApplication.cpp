#include "MyApplication.h"
#include <iostream>

int MyApplication::Run()
{
	Init();

	while (true)
	{
		Update();


		Draw(fCam->GetProjectionView());
	}

	Destroy();

	return 0;
}

void MyApplication::setCamera()
{
	fCam = new FlyCamera;
	fCam->m_pWindow = m_window;

	fCam->SetTransform(mat4(1));
	fCam->SetPosition(vec3(15, 20, 10));
	fCam->SetLookAt(vec3(30, 20, 25), vec3(0, 14, 0), vec3(0, 1, 0));
	fCam->SetupPerspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);

	fCam->SetFlySpeed(25);

	createWindow("Application", 1280, 720);
	fCam->SetInputWindow(m_window);
}

bool MyApplication::Init()
{
	setCamera();

	m_clearColour = vec3(0.5, 0.5, 0.5);

	deltaTime = 0;
	previousTime = 0;


	physicsScene = new PhysicsScene();
	physicsScene->gravity = vec3(0, -9.8f, 0);
	physicsScene->timeStep = 0.1;




	float ballRadius = 1;
	float mass = 3;
	ball1 = new SphereClass(vec3(0, 30, 0), vec3(0, 0, 0), mass, ballRadius, false,
		glm::vec4(1, 0, 0, 1));
	physicsScene->addActor(ball1);
	
	ball2 = new SphereClass(vec3(15, 20, 0), vec3(0, 0, 0), mass, ballRadius, true,
		glm::vec4(0, 0, 1, 1));
	physicsScene->addActor(ball2);
	
	ball3 = new SphereClass(vec3(-15, 20, 0), vec3(0, 0, 0), mass, ballRadius, true,
		glm::vec4(0, 1, 0, 1));
	physicsScene->addActor(ball3);
	
	SpringJoint* spring1 = new SpringJoint(ball1, ball2, 5, 0.05);
	physicsScene->addActor(spring1);
	SpringJoint* spring2 = new SpringJoint(ball1, ball3, 5, 0.05);
	physicsScene->addActor(spring2);


	




	ball = new SphereClass(vec3(0, 200, 1), vec3(0, 0, 0), 10, 3, false, 
		vec4(0, 1, 0, 1));
	
	physicsScene->addActor(ball);


	staticBox = new BoxClass(vec3(-20, 1, 10), vec3(0, 0, 0), true, 3, vec3(1, 1, 1), vec4(0, 1, 0, 1));
	aquabox = new BoxClass(vec3(10, 100, 15), vec3(0, 1, 0), false, 1, vec3(1, 1, 1), vec4(0, 1, 1, 1));
	moxbox = new BoxClass(vec3(10, 50, 15), vec3(0, 1, 0), false, 3, vec3(1, 1, 1), vec4(1, 0, 1, 1));
	physicsScene->addActor(staticBox);
	physicsScene->addActor(aquabox);
	physicsScene->addActor(moxbox);

	sphere = new SphereClass(vec3(-20, 150, 10), vec3(0, 0, 0), 3, 3, false,
		vec4(0, 1, 0, 1));	
	physicsScene->addActor(sphere);




	testBox = new BoxClass(vec3(-20, 250, -20), vec3(0, 0, 0), false, 3, vec3(1, 1, 1), vec4(1, 1, 0, 1));
	physicsScene->addActor(testBox);

	testSphere = new SphereClass(vec3(-20, -1, -20), vec3(0, 0, 0), 10, 3, true,
		vec4(1, 0, 1, 1));
	physicsScene->addActor(testSphere);
	





	plane = new PlaneClass(vec3(0, 1, 0), -10);
	physicsScene->addActor(plane);

	Gizmos::create();

	return true;
}

bool MyApplication::Update()
{
	float currentTime = (float)glfwGetTime();
	deltaTime = currentTime - previousTime;
	previousTime = currentTime;

	fCam->Update(deltaTime);


	// if mouse click
	if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
	physicsScene->addActor(new SphereClass(fCam->GetPosition(), vec3(0), 3, 3, false, vec4(1, 1, 1, 0)));


	physicsScene->update();

	return true;
}

void MyApplication::Draw(const mat4& m_projectionViewMatrix)
{
	glClearColor(m_clearColour.r, m_clearColour.g,
		m_clearColour.b, 1);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	Gizmos::clear();

	Gizmos::addLine(vec3(10, -9.9f, 0), vec3(-10, -9.9f, 0), vec4(1, 1, 1, 1));
	Gizmos::addLine(vec3(0, -9.9f, 10), vec3(0, -9.9f, -10), vec4(1, 1, 1, 1));

	physicsScene->addGizmos();



	Gizmos::draw(fCam->GetProjectionView());


	glfwSwapBuffers(m_window);
	glfwPollEvents();
}

void MyApplication::Destroy()
{

}

bool MyApplication::createWindow(const char* title, int width, int height)
{
	if (glfwInit() == GL_FALSE)
		return false;

	m_window = glfwCreateWindow(1280, 720, "Application", nullptr, nullptr);
	if (m_window == nullptr)
	{
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(m_window);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
		return false;
	}

	glfwSetWindowSizeCallback(m_window, [](GLFWwindow*, int w, int h) { glViewport(0, 0, w, h); });

	auto major = ogl_GetMajorVersion();
	auto minor = ogl_GetMinorVersion();
	std::cout << "GL: " << major << "." << minor << std::endl;

	glClearColor(0.25f, 0.25f, 0.25f, 1);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	return true;
}

void MyApplication::destroyWindow()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
	ImGui_ImplGlfwGL3_Shutdown();
}