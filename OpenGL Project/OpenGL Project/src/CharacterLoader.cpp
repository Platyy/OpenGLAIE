#include "CharacterLoader.h"


CharacterLoader::CharacterLoader()
{
}

CharacterLoader::~CharacterLoader()
{
}

void CharacterLoader::Init(const GLchar* vsSource, const GLchar* fsSource)
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
	if (m_FBX->load("./data/Models/Characters/Enemytank/EnemyTank.fbx"))
	{
		CreateOpenGLBuffers(m_FBX);
		m_FBX->initialiseOpenGLTextures();
		m_WorldMatrix = glm::scale(m_WorldMatrix, glm::vec3(0.1f));
	}
}

void CharacterLoader::UpdateCharPos(glm::vec3 pos, glm::vec3 rot)
{
	m_WorldMatrix =
		glm::scale(glm::vec3(0.1f)) *
		glm::rotate((rot.x * 0.0174533f) * 0.5f, glm::vec3(1, 0 ,0)) *
		glm::rotate((rot.y * 0.0174533f) * 0.5f, glm::vec3(0, 1, 0)) *
		glm::rotate((rot.z * 0.0174533f) * 0.5f, glm::vec3(0, 0, 1)) *
		glm::translate(pos);
}

void CharacterLoader::Render(glm::mat4 projectionViewMatrix, glm::vec3 cameraPos)
{
	glUseProgram(m_ProgramID);
	unsigned int projViewUniform = glGetUniformLocation(m_ProgramID, "ProjectionView");
	unsigned int worldUniform = glGetUniformLocation(m_ProgramID, "World");
	glUniformMatrix4fv(projViewUniform, 1, GL_FALSE, glm::value_ptr(projectionViewMatrix * m_WorldMatrix));
	glUniformMatrix4fv(worldUniform, 1, GL_FALSE, glm::value_ptr(m_WorldMatrix));

	//Default light
	glm::vec3 light(sin(glfwGetTime()), 1, cos(glfwGetTime()));
	GLint lightColor = glGetUniformLocation(m_ProgramID, "LightColor");
	GLint lightDirection = glGetUniformLocation(m_ProgramID, "LightDir");
	glUniform3f(lightDirection, light.x, light.y, light.z);
	glUniform3f(lightColor, 1.0, 1.0, 1.0);

	FBXSkeleton* skeleton = m_FBX->getSkeletonByIndex(0);
	int _boneLoc = glGetUniformLocation(m_ProgramID, "bones");
	glUniformMatrix4fv(_boneLoc, skeleton->m_boneCount, GL_FALSE, (float*)skeleton->m_bones);

	FBXAnimation* animation = m_FBX->getAnimationByIndex(0);
	skeleton->evaluate(animation, (float)glfwGetTime(), true, 24.0f);
	skeleton->updateBones();

	for (unsigned int bone_index = 0; bone_index < skeleton->m_boneCount; ++bone_index)
		skeleton->m_nodes[bone_index]->updateGlobalTransform();

	for (unsigned int i = 0; i < m_FBX->getMeshCount(); ++i)
	{
		FBXMeshNode* mesh = m_FBX->getMeshByIndex(i);

		FBXTexture* pDiffTex = mesh->m_material->textures[FBXMaterial::DiffuseTexture];
		FBXTexture* pNormTex = mesh->m_material->textures[FBXMaterial::NormalTexture];
		if (pDiffTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, pDiffTex->handle);
			int tex = glGetUniformLocation(m_ProgramID, "diffuse");
			glUniform1i(tex, 0);
		}
		if (pNormTex) {
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, pNormTex->handle);
			int tex = glGetUniformLocation(m_ProgramID, "normalMap");
			glUniform1i(tex, 1);
		}
		unsigned int* glData = (unsigned int*)mesh->m_userData;
		glBindVertexArray(glData[0]);
		glDrawElements(GL_TRIANGLES, (unsigned int)mesh->m_indices.size(), GL_UNSIGNED_INT, 0);
	}
}

void CharacterLoader::CreateOpenGLBuffers(FBXFile* fbx)
{
	for (unsigned int i = 0; i < fbx->getMeshCount(); ++i)
	{
		FBXMeshNode* mesh = fbx->getMeshByIndex(i);
		unsigned int* glData = new unsigned int[3];

		int g = sizeof(FBXVertex);

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

		glEnableVertexAttribArray(0); //position
		glEnableVertexAttribArray(1); //normals
		glEnableVertexAttribArray(2); //tangents
		glEnableVertexAttribArray(3); //texcoords
		glEnableVertexAttribArray(4); //weights
		glEnableVertexAttribArray(5); //indices
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex),
			(void*)FBXVertex::PositionOffset);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(FBXVertex),
			(void*)FBXVertex::NormalOffset);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, sizeof(FBXVertex),
			(void*)FBXVertex::TangentOffset);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(FBXVertex),
			(void*)FBXVertex::TexCoord1Offset);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex),
			(void*)FBXVertex::WeightsOffset);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex),
			(void*)FBXVertex::IndicesOffset);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		mesh->m_userData = glData;
	}
}

void CharacterLoader::CleanUpOpenGLBuffers(FBXFile* fbx)
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