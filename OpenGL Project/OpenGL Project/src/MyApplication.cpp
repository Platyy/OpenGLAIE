#include "MyApplication.h"
#include "ShaderLoader.h"

MyApplication::MyApplication()
{
	camera = new Camera();
	quat = new Quaternions();
	render = new RenderEngine();
	loader = new ModelLoader();
	texLoader = new TexLoader();
	charLoader = new CharacterLoader();
	otherChar = new CharacterLoader();
	gui = new GUI();
	terrain = new Terrain();
}

MyApplication::~MyApplication()
{
	delete camera;
	delete quat;
	delete render;
	delete loader;
	delete charLoader;
	delete otherChar;
	delete texLoader;
	delete gui;
	delete terrain;
}

int MyApplication::Init()
{
	if (glfwInit() == false)
		return -1;

	window =
		glfwCreateWindow(1280, 720, "Computer Graphics", nullptr, nullptr);

	if (window == nullptr)
	{
		glfwTerminate();
		return -2;
	}

	glfwMakeContextCurrent(window);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(window);
		glfwTerminate();
		return -3;
	}

	std::string v = Resource::ImportShader("vertexModel");
	std::string f = Resource::ImportShader("fragmentModel");

	vsSource = v.c_str();
	fsSource = f.c_str();

	std::string tV = Resource::ImportShader("terrainVS");
	std::string tF = Resource::ImportShader("terrainFS");
	const char* tVS = tV.c_str();
	const char* tFS = tF.c_str();

	render->Init(vsSource, fsSource);
	loader->Init(vsSource, fsSource);
//	texLoader->Init(vsSource, fsSource);
	charLoader->Init(vsSource, fsSource);
	gui->Init(window);
	terrain->GenGrid(64, 64, tVS, tFS);

	prog = render->m_ProgramID;

	Gizmos::create();
	camera->SetPerspective(0.25f, 16.f / 9.f, 0.1f, 1000.f);
	camera->SetPosition(vec3(10, 5, 10));
	camera->SetLookAt(vec3(0, 0, 0));
	camera->m_Speed = 25;
	quat->Init();


	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	return 1;
}

bool MyApplication::Update()
{
	glfwPollEvents();
	m_Update = true;
	camera->UpdatePos(window);

	sphere.center = vec3(10, 0, 0);
	sphere.radius = 0.5f;

	int state = glfwGetKey(window, GLFW_KEY_ESCAPE);

	if (state == GLFW_PRESS)
	{
		m_Update = false;
	}

	return m_Update;
}

void MyApplication::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Gizmos::draw(camera->GetProjectionView());
	Gizmos::addSphere(sphere.center, sphere.radius, 8, 8, vec4(1, 0, 1, 1));
	vec4 plane(0, 1, 0, -1);

	float d = glm::dot(vec3(plane), sphere.center) + plane.w;
	vec4 planeColour(1, 1, 0, 1);
	if (d > sphere.radius)
		planeColour = vec4(0, 1, 0, 1);
	else if (d < -sphere.radius)
		planeColour = vec4(1, 0, 0, 1);
	Gizmos::addTri(vec3(4, 1, 4), vec3(-4, 1, -4), vec3(-4, 1, 4), planeColour);
	Gizmos::addTri(vec3(4, 1, 4), vec3(4, 1, -4), vec3(-4, 1, -4), planeColour);

	vec4 planes[6];
	sphere.GetFrustumPlanes(camera->GetProjectionView(), planes);
	glm::vec3 m_pos = glm::vec3(camera->GetWorldTransform()[3].x, camera->GetWorldTransform()[3].y, camera->GetWorldTransform()[3].z);
	for (int i = 0; i < 6; i++) {
		float d = glm::dot(vec3(planes[i]), sphere.center) +
			planes[i].w;
		if (d < -sphere.radius) {
			printf("Behind, don't render it!\n");
			break;
		}
		else if (d < sphere.radius) {
			printf("Touching, we still need to render it!\n");
		}
		else {
			printf("Front, fully visible so render it!\n");
			charLoader->Render(camera->GetProjectionView(), m_pos);
			charLoader->UpdateCharPos(glm::vec3(gui->pos.x, gui->pos.y, gui->pos.z), 
				glm::vec3(gui->rot.x, gui->rot.y, gui->rot.z));
		}
	}

//	texLoader->Render(camera->GetProjectionView(), texLoader->m_WorldMatrix);
	loader->Render(camera->GetProjectionView(), loader->m_WorldMatrix, m_pos);

	terrain->RenderGrid(camera->GetProjectionView(), gui->m_OffsetGrass, gui->m_OffsetRocks);

	render->Render(camera->GetProjectionView(), loader->m_WorldMatrix);
	gui->Draw();
	glfwSwapBuffers(window);
}

void MyApplication::Close()
{
	Gizmos::destroy();
	loader->CleanUpOpenGLBuffers(loader->m_FBX);
//	texLoader->Close();
	charLoader->CleanUpOpenGLBuffers(charLoader->m_FBX);
	render->Close();
	gui->Close();
	glfwDestroyWindow(window);
	glfwTerminate();
}