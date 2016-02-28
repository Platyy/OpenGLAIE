#include <glm\gtc\quaternion.hpp>
#include <glm\gtx\quaternion.hpp>
#include <GLFW\glfw3.h>
#include <glm\gtx\transform.hpp>
#include "Gizmos.h"
#include <iostream>
#include <float.h>

#pragma once
class Quaternions
{
public:
	glm::vec3 m_Positions[2];
	glm::quat m_Rotations[2];

	bool reached = false;

	struct KeyFrame
	{
		glm::vec3 m_Pos;
		glm::quat m_Rot;
	};

	KeyFrame m_HipFrames[2];
	KeyFrame m_KneeFrames[2];
	KeyFrame m_AnkleFrames[2];

	glm::mat4 m_HipBone;
	glm::mat4 m_KneeBone;
	glm::mat4 m_AnkleBone;

	void Init();
	void Update();
};