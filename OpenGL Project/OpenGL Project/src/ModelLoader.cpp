#include "ModelLoader.h"
#include <glm\gtc\type_ptr.hpp>

ModelLoader::ModelLoader() : m_WorldMatrix(glm::mat4(1))
{
}

ModelLoader::~ModelLoader()
{
	delete m_FBX;
}

void ModelLoader::Init(const GLchar* vsSource, const GLchar* fsSource)
{
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, (const char**)&vsSource, 0);
	glCompileShader(vertexShader);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, (const char**)&fsSource, 0);
	glCompileShader(fragmentShader);
	m_ProgramID = glCreateProgram();
	glAttachShader(m_ProgramID, vertexShader);
	glAttachShader(m_ProgramID, fragmentShader);
	glLinkProgram(m_ProgramID);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	m_FBX = new FBXFile();
	if (m_FBX->load("./data/Models/Buddha.fbx"))
	{
		CreateOpenGLBuffers(m_FBX);
		m_WorldMatrix = glm::scale(m_WorldMatrix, glm::vec3(0.5f));
	}
}

void ModelLoader::CreateOpenGLBuffers(FBXFile* fbx)
{
	for (unsigned int i = 0; i < fbx->getMeshCount(); ++i)
	{
		FBXMeshNode* mesh = fbx->getMeshByIndex(i);

		unsigned int* glData = new unsigned int[3];

		glGenVertexArrays(1, &glData[0]);
		glBindVertexArray(glData[0]);

		glGenBuffers(1, &glData[1]);
		glGenBuffers(1, &glData[2]);

		glBindBuffer(GL_ARRAY_BUFFER, glData[1]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glData[2]);

		glBufferData(GL_ARRAY_BUFFER, 
			mesh->m_vertices.size() * sizeof(FBXVertex),
			mesh->m_vertices.data(), GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			mesh->m_indices.size() * sizeof(unsigned int),
			mesh->m_indices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), 0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(FBXVertex), ((char*)0) + FBXVertex::NormalOffset);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		mesh->m_userData = glData;
	}
}

void ModelLoader::CleanUpOpenGLBuffers(FBXFile* fbx)
{
	for (unsigned int i = 0; i < fbx->getMeshCount(); ++i)
	{
		FBXMeshNode* mesh = fbx->getMeshByIndex(i);
		unsigned int* glData = (unsigned int*)mesh->m_userData;
		glDeleteVertexArrays(1, &glData[0]);
		glDeleteBuffers(1, &glData[1]);
		glDeleteBuffers(1, &glData[2]);
		delete[] glData;
	}
}

void ModelLoader::Render(glm::mat4 projectionViewMatrix, glm::mat4 world, glm::vec3 cameraPos)
{
	glUseProgram(m_ProgramID);
	unsigned int projViewUniform = glGetUniformLocation(m_ProgramID, "ProjectionView");
	worldUniform = glGetUniformLocation(m_ProgramID, "World");
	glUniformMatrix4fv(projViewUniform, 1, GL_FALSE, glm::value_ptr(projectionViewMatrix));
	glUniformMatrix4fv(worldUniform, 1, false, glm::value_ptr(world));

	//Default light
	unsigned int loc = glGetUniformLocation(m_ProgramID, "diffuse");
	glUniform1i(loc, 0);
	loc = glGetUniformLocation(m_ProgramID, "normalMap");
	glUniform1i(loc, 1);

	GLint lightColor = glGetUniformLocation(m_ProgramID, "LightColor");
	GLint lightDirection = glGetUniformLocation(m_ProgramID, "LightDir");
	glUniform3f(lightDirection, 0.0, 1.0, 0.0);
	glUniform3f(lightColor, 1.0, 1.0, 1.0);

	for (unsigned int i = 0; i < m_FBX->getMeshCount(); ++i)
	{
		FBXMeshNode* mesh = m_FBX->getMeshByIndex(i);
		unsigned int* glData = (unsigned int*)mesh->m_userData;
		glBindVertexArray(glData[0]);
		glDrawElements(GL_TRIANGLES, (unsigned int)mesh->m_indices.size(), GL_UNSIGNED_INT, 0);
	}
}
