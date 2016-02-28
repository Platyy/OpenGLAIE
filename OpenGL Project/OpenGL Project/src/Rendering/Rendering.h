#include "gl_core_4_4.h"
#include "GLFW\glfw3.h"
#include "glm\glm.hpp"
#include <math.h>
#include <iostream>

#pragma once
class Rendering
{
public:
	Rendering();
	~Rendering();

	// Don't copy render data
	Rendering(const Rendering& other);
	// Move it
	Rendering(Rendering&& other);

	unsigned int GetVAO() const { return m_VAO; }
	unsigned int GetVBO() const { return m_VBO; }
	unsigned int GetIBO() const { return m_IBO; }

	enum Buffers : unsigned char
	{
		VAO = 1,
		VBO = 2,
		IBO = 4,
		ALL = 7
	};

	struct Vertex 
	{
		glm::vec4 m_Position;
		glm::vec4 m_Colour;
	};

	void GenBuffers(unsigned char buffers);
	void ClearBuffers(unsigned char buffers);

	void CheckBuffers(unsigned char& buffers);

	void Bind();
	void Render();
	void Unbind();

	void SetIndexCount(unsigned int count) { m_IndexCount = count; }

	void GenerateGrid(unsigned int _rows, unsigned int _cols);

private:
	unsigned int m_VAO; // Vertex Array Object
	unsigned int m_VBO; // Vertex Buffer Object (Stores vertex information)
	unsigned int m_IBO; // Index Buffer Object
	unsigned int m_IndexCount;
};