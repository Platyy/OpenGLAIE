#pragma once
#include <stb_image.h>
#include "gl_core_4_4.h"
#include "GLFW\glfw3.h"
#include "glm\glm.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtc\noise.hpp"
#include <iostream>

struct KeyState
{
	enum State
	{
		UP,
		PRESSED,
		DOWN,
		RELEASED,
		STATE_COUNT
	};
};

class Terrain
{
public:
	Terrain();
	~Terrain();

	GLuint m_VBO, m_VAO, m_IBO;
	GLuint indexCount;

	struct Verts
	{
		glm::vec4 position;
		glm::vec2 texCoord;
	};

	unsigned int m_ProgramID;

	unsigned int m_PerlinTexture, m_RockTexture, m_GrassTexture;

	void Init(const GLchar* vsSource, const GLchar* fsSource);

	void GenGrid(unsigned int _rows, unsigned int _cols, const char* terrainVS, const char* terrainFS);

	void ReGenGrid(unsigned int _rows, unsigned int _cols);

	void UpdateGrid(GLFWwindow* window);

	KeyState::State HandleInput(GLFWwindow* window, int key);

	void RenderGrid(glm::mat4 projView, float grass, float rock);
};



