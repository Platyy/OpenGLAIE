#include <gl_core_4_4.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "Rendering.h"
#include <iostream>

#pragma once
class Shader
{
public:

	unsigned int m_ProgramID;

	const char* vsSource;

	const char* fsSource;

	unsigned int indexCount;

	int success;

	unsigned int m_VertexShader;
	unsigned int m_FragmentShader;

	Rendering* render;

	void Update();
	void ShaderStart(glm::mat4 projectionViewMatrix, unsigned int VAO);
	~Shader();
};