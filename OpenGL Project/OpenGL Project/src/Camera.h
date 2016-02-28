#include <iostream>
#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include <Gizmos.h>

using glm::mat4;

#pragma once
class Camera
{
public:
	float m_Speed = 1;
	glm::vec3 previousMouse;
	glm::vec3 currentMouse;


	float m_PreviousTime;


	mat4 m_WorldTransform;
	mat4 m_ViewTransform;
	mat4 m_ProjectionTransform;
	mat4 m_ProjectionViewTransform;

	void Update();

	void UpdatePos(GLFWwindow* window);

	void SetPerspective(float FOV, float aspectRatio, float nearField, float farField);
	void SetLookAt(glm::vec3 to);
	void SetPosition(glm::vec3 position);

	mat4 GetWorldTransform();
	mat4 GetView();
	mat4 GetProjection();
	mat4 GetProjectionView();
};