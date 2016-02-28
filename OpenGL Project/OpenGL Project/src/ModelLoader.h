#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

#include "FBXFile.h"
#include "gl_core_4_4.h"
#include "glm\glm.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtc\matrix_transform.hpp"


#pragma once
class ModelLoader
{
public:

	glm::mat4 m_WorldMatrix;

	unsigned int m_VertAttribs;
	FBXMaterial* m_Mat;
	std::vector<FBXVertex> m_Verts;
	std::vector<unsigned int> m_Indices;
	glm::mat4 m_LocalTransform;
	glm::mat4 m_GlobalTransform;

	void* m_UserData;

	void CreateOpenGLBuffers(FBXFile* fbx);
	void CleanUpOpenGLBuffers(FBXFile* fbx);

	FBXFile* m_FBX;
	unsigned int m_ProgramID;

	glm::vec3 colour;

	void Init(const GLchar* vsSource, const GLchar* fsSource);
	void Render(glm::mat4 projViewMatrix, glm::mat4 world, glm::vec3 cameraPos);

	ModelLoader();
	~ModelLoader();

private:
	unsigned int worldUniform;
};

#endif // !MODEL_LOADER_H
