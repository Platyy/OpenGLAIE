#include "Quaternions.h"

void Quaternions::Init()
{
	m_Positions[0] = glm::vec3(10, 5, 10);
	m_Positions[1] = glm::vec3(-10, 0, -10);
	m_Positions[2] = glm::vec3(20, 10, 0);
	m_Rotations[0] = glm::quat(glm::vec3(1, -1, 0));
	m_Rotations[1] = glm::quat(glm::vec3(0, 1, 0));
	m_Rotations[2] = glm::quat(glm::vec3(1, 0, 0));

	m_HipFrames[0].m_Pos = glm::vec3(0, 5, 0);
	m_HipFrames[0].m_Rot = glm::quat(glm::vec3(1, 0, 0));
	m_HipFrames[1].m_Pos = glm::vec3(0, 5, 0);
	m_HipFrames[1].m_Rot = glm::quat(glm::vec3(-1, 0, 0));

	m_KneeFrames[0].m_Pos = glm::vec3(0, -2.5f, 0);
	m_KneeFrames[0].m_Rot = glm::quat(glm::vec3(1, 0, 0));
	m_KneeFrames[1].m_Pos = glm::vec3(0, -2.5f, 0);
	m_KneeFrames[1].m_Rot = glm::quat(glm::vec3(0, 0, 0));

	m_AnkleFrames[0].m_Pos = glm::vec3(0, -2.5f, 0);
	m_AnkleFrames[0].m_Rot = glm::quat(glm::vec3(-1, 0, 0));
	m_AnkleFrames[1].m_Pos = glm::vec3(0, -2.5f, 0);
	m_AnkleFrames[1].m_Rot = glm::quat(glm::vec3(0, 0, 0));
}

void Quaternions::Update()
{
	//// Get time scale
	//float scale = glm::cos((float)glfwGetTime()) * 0.5f + 0.5f;
	////Check for start
	//if (reached && scale > 0.9f)
	//{
	//	reached = false;
	//}
	//// Simple lerp
	//glm::vec3 p = (1.0f - scale) * m_Positions[0] + scale * m_Positions[1];
	//// Slerp the two rotations
	//glm::quat r = glm::slerp(m_Rotations[0], m_Rotations[1], scale);
	//// Check for end
	//if (scale < 0.0001f)
	//{
	//	reached = true;
	//}

	//// Make matrix
	//glm::mat4 m = glm::translate(p) * glm::toMat4(r);
	//// Draw transform and box
	//Gizmos::addTransform(m_HipBone);
	//Gizmos::addAABBFilled(p, glm::vec3(.5f), glm::vec4(1, 0, 0, 1), &m_HipBone);

	float scale = glm::cos((float)glfwGetTime()) * 0.5f + 0.5f;

	glm::vec3 hipVec = (1.0f - scale) * m_HipFrames[0].m_Pos + scale * m_HipFrames[1].m_Pos;
	glm::quat hipRot = glm::slerp(m_HipFrames[0].m_Rot, m_HipFrames[1].m_Rot, scale);
	m_HipBone = glm::translate(hipVec) * glm::toMat4(hipRot);

	glm::vec3 kneeVec = (1.0f - scale) * m_KneeFrames[0].m_Pos + scale * m_KneeFrames[1].m_Pos;
	glm::quat kneeRot = glm::slerp(m_KneeFrames[0].m_Rot, m_KneeFrames[1].m_Rot, scale);
	m_KneeBone = m_HipBone * (glm::translate(kneeVec) * glm::toMat4(kneeRot));

	glm::vec3 ankleVec = (1.0f - scale) * m_AnkleFrames[0].m_Pos + scale * m_AnkleFrames[1].m_Pos;
	glm::quat ankleRot = glm::slerp(m_AnkleFrames[0].m_Rot, m_AnkleFrames[1].m_Rot, scale);
	m_AnkleBone = m_KneeBone * (glm::translate(ankleVec) * glm::toMat4(ankleRot));

	glm::vec3 hipPos = glm::vec3(m_HipBone[3].x, m_HipBone[3].y, m_HipBone[3].z);
	glm::vec3 kneePos = glm::vec3(m_KneeBone[3].x, m_KneeBone[3].y, m_KneeBone[3].z);
	glm::vec3 anklePos = glm::vec3(m_AnkleBone[3].x, m_AnkleBone[3].y, m_AnkleBone[3].z);

	glm::vec3 half(.75f);
	glm::vec4 pink(1, 0, 1, 1);

	Gizmos::addAABBFilled(hipPos, half, pink, &m_HipBone);
	Gizmos::addAABBFilled(kneePos, half, pink, &m_KneeBone);
	Gizmos::addAABBFilled(anklePos, half, pink, &m_AnkleBone);
}