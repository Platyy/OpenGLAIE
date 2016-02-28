#include <stb_image.h>
#include "gl_core_4_4.h"
#include "glm\glm.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "GLFW\glfw3.h"

struct Vertex 
{
	float x, y, z, w;
	float nx, ny, nz, nw;
	float tx, ty, tz, tw;
	float s, t;
};
#pragma once
class TexLoader
{
public:
	glm::mat4 m_WorldMatrix;

	unsigned int m_VAO, m_VBO, m_IBO;

	unsigned int m_Texture;
	unsigned int m_Normal;
	unsigned int m_ProgramID;
	void Init(const char* vsSource, const char* fsSource);
	void Render(glm::mat4 projectionViewMatrix, glm::mat4 world);
	void Close();
	TexLoader();
	~TexLoader();
};

