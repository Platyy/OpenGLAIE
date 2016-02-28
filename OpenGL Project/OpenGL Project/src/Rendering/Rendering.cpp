//#include "Rendering.h"
//
//void Rendering::GenerateGrid(unsigned int _rows, unsigned int _cols)
//{
//	Vertex* aoVertices = new Vertex[_rows * _cols];
//	for (unsigned int r = 0; r < _rows; ++r)
//	{
//		for (unsigned int c = 0; c < _cols; ++c)
//		{
//			aoVertices[r * _cols + c].m_Position = glm::vec4((float)c, 0, (float)r, 1);
//
//			glm::vec3 colour = glm::vec3(sinf((c / (float)(_cols - 1)) * (r / (float)(_rows - 1))));
//			aoVertices[r * _cols + c].m_Colour = glm::vec4(colour, 1);
//		}
//	}
//	// Index array
//	unsigned int* auiIndices = new unsigned int[(_rows - 1) * (_cols - 1) * 6];
//	unsigned int index = 0;
//	for (unsigned int r = 0; r < (_rows - 1); ++r)
//	{
//		for (unsigned int c = 0; c < (_cols - 1); ++c)
//		{
//			auiIndices[index++] = r * _cols + c;
//			auiIndices[index++] = (r + 1) * _cols + c;
//			auiIndices[index++] = (r + 1) * _cols + (c + 1);
//
//			auiIndices[index++] = r * _cols + c;
//			auiIndices[index++] = (r + 1) * _cols + (c + 1);
//			auiIndices[index++] = r * _cols + (c + 1);
//		}
//	}
//
//	// Handling Vertex Buffer
//	glGenBuffers(1, &m_VBO);
//	glGenBuffers(1, &m_IBO);
//
//	// Generate Vertex Array Object
//	glGenVertexArrays(1, &m_VAO);
//	glBindVertexArray(m_VAO);
//
//	// Populating VBO
//	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
//	glBufferData(GL_ARRAY_BUFFER, (_rows * _cols) * sizeof(Vertex), aoVertices, GL_STATIC_DRAW);
//	glEnableVertexAttribArray(0);
//	glEnableVertexAttribArray(1);
//	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
//	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4)));
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	
//	// Populating IBO
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (_rows - 1) * (_cols - 1) * 6 * sizeof(unsigned int), auiIndices, GL_STATIC_DRAW);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//
//	glBindVertexArray(0);
//
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//
//	delete[] aoVertices;
//}
//
//void Rendering::Render()
//{
//	//material
//		//programid
//	//mesh
//		//vao
//		//indexcount
//
//	glUseProgram(material.programid);
//
//	for (int i = 0;)
//	{
//		glBindVertexArray(meshes[i].vao);
//		glDrawElements(meshes[i].indexcount)
//	}
//
//
//}

#include "Rendering.h"

Rendering::Rendering() : m_IBO(0), m_VBO(0), m_VAO(0)
{
}

Rendering::~Rendering()
{
	ClearBuffers(Buffers::ALL);
}

Rendering::Rendering(const Rendering& other)
{
	m_VAO = other.m_VAO;
	m_VBO = other.m_VBO;
	m_IBO = other.m_IBO;
}

Rendering::Rendering(Rendering&& other)
{
	m_VAO = other.m_VAO;
	m_VBO = other.m_VBO;
	m_IBO = other.m_IBO;

	other.m_VAO = 0;
	other.m_VBO	= 0;
	other.m_IBO	= 0;
}

void Rendering::GenBuffers(unsigned char buffers)
{
	// Make sure to delete existing buffers of same type to avoid leaks
	ClearBuffers(buffers);

	if (buffers & Buffers::VAO)
	{
		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);
	}

	if (buffers & Buffers::VBO)
	{
		glGenBuffers(1, &m_VBO);
		if (buffers & Buffers::VAO)
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	}

	if (buffers & Buffers::IBO)
	{
		glGenBuffers(1, &m_IBO);
		if (buffers & Buffers::IBO)
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	}
	if (buffers & Buffers::VAO)
		glBindVertexArray(0);
}

void Rendering::ClearBuffers(unsigned char buffers)
{
	if (buffers & Buffers::VAO && m_VAO > 0)
	{
		glDeleteVertexArrays(1, &m_VAO);
	}
	if (buffers & Buffers::VBO && m_VBO > 0)
	{
		glDeleteBuffers(1, &m_VBO);
	}
	if (buffers & Buffers::IBO && m_IBO > 0)
	{
		glDeleteBuffers(1, &m_IBO);
	}
}

void Rendering::CheckBuffers(unsigned char& buffers)
{
	// Using bitwise to check if m_XXX or Buffer::XXX have values
	// Then buffers = 1 if either have vals
	buffers = 0;
	buffers |= (m_VAO > 0) ? Buffers::VAO : 0;
	buffers |= (m_VBO > 0) ? Buffers::VBO : 0;
	buffers |= (m_IBO > 0) ? Buffers::IBO : 0;
}

void Rendering::Render()
{
	if (m_IBO)
		glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, 0);
}

void Rendering::Bind()
{
	if (m_VAO > 0)
	{
		glBindVertexArray(m_VAO);
	}
	else if (m_VBO > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		if (m_IBO > 0)
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	}
}

void Rendering::Unbind()
{
	if (m_VAO > 0)
		glBindVertexArray(0);
	else if (m_VBO > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		if (m_IBO > 0)
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

void Rendering::GenerateGrid(unsigned int _rows, unsigned int _cols)
{
	Vertex* aoVertices = new Vertex[_rows * _cols];
	for (unsigned int r = 0; r < _rows; ++r)
	{
		for (unsigned int c = 0; c < _cols; ++c)
		{
			aoVertices[r * _cols + c].m_Position = glm::vec4((float)c, 0, (float)r, 1);

			//glm::vec3 colour = glm::vec3(sinf((c / (float)(_cols - 1)) * (r / (float)(_rows - 1))));
			glm::vec3 colour = glm::vec3(1, 0, 0);
			aoVertices[r * _cols + c].m_Colour = glm::vec4(colour, 1);
		}
	}
	// Index array
	unsigned int* auiIndices = new unsigned int[(_rows - 1) * (_cols - 1) * 6];
	unsigned int index = 0;
	for (unsigned int r = 0; r < (_rows - 1); ++r)
	{
		for (unsigned int c = 0; c < (_cols - 1); ++c)
		{
			auiIndices[index++] = r * _cols + c;
			auiIndices[index++] = (r + 1) * _cols + c;
			auiIndices[index++] = (r + 1) * _cols + (c + 1);

			auiIndices[index++] = r * _cols + c;
			auiIndices[index++] = (r + 1) * _cols + (c + 1);
			auiIndices[index++] = r * _cols + (c + 1);
		}
	}
	GenBuffers(Buffers::VBO);
	GenBuffers(Buffers::IBO);
	Bind();

	delete[] aoVertices;
}