#ifndef CHARACTER_LOADER_H
#define CHARACTER_LOADER_H

#include "FBXFile.h"
#include <stb_image.h>
#include "gl_core_4_4.h"
#include "glm\glm.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "GLFW\glfw3.h"
#include "glm\gtx\transform.hpp"

#pragma once
class CharacterLoader
{
public:
	unsigned int m_VAO, m_VBO, m_IBO;

	unsigned int m_Texture;
	FBXMaterial m_Material;

	void Init(const GLchar* vsSource, const GLchar* fsSource);
	void Render(glm::mat4 projectionViewMatrix, glm::vec3 cameraPos);

	void CreateOpenGLBuffers(FBXFile* fbx);
	void CleanUpOpenGLBuffers(FBXFile* fbx);

	void* m_UserData;

	glm::mat4 m_WorldMatrix;

	void UpdateCharPos(glm::vec3 pos, glm::vec3 rot);

	FBXFile* m_FBX;
	unsigned int m_ProgramID;

	CharacterLoader();
	~CharacterLoader();
};


#endif // !CHARACTER_LOADER_H
