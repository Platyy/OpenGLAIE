#pragma once
#include <iostream>
#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include <Gizmos.h>
#include "Camera.h"
#include "Quaternions.h"
#include "Rendering\RenderEngine.h"
#include "ModelLoader.h"
#include "Rendering\TexLoader.h"
#include "CharacterLoader.h"
#include "Rendering\GUI.h"
#include "Terrain.h"
#include "Collision.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::rotate;



class MyApplication
{
public:
	mat4 sun;
	mat4 merc;
	mat4 venus;

	vec3 sun_pos;
	vec3 merc_pos;
	vec3 venus_pos;

	mat4 projection;
	mat4 view;

	GLFWwindow* window;

	GLuint prog;

	bool m_Update;
	const char* vsSource;
	const char* fsSource;

	const char* tVS;
	const char* tFS;
	bool needInit = true;

	int Init();
	bool Update();
	void Draw();
	void Close();

	MyApplication();
	~MyApplication();

	//awkward
	Camera* camera;
	Quaternions* quat;
	RenderEngine* render;
	ModelLoader* loader;
	CharacterLoader* charLoader;
	CharacterLoader* otherChar;
	TexLoader* texLoader;
	GUI* gui;
	Terrain* terrain;
	BoundingSphere sphere;
};

