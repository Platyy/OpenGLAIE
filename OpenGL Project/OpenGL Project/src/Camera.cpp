#include "Camera.h"

void Camera::SetPerspective(float FOV, float aspectRatio, float nearField, float farField) 
{
	m_ProjectionTransform = glm::perspective(glm::pi<float>() * FOV,
		aspectRatio, nearField, farField);
}

void Camera::SetLookAt(glm::vec3 to)
{
	glm::vec3 temp(GetWorldTransform()[3].x, GetWorldTransform()[3].y, GetWorldTransform()[3].z);
	m_ViewTransform = glm::lookAt(temp, to, glm::vec3(0, 1, 0));
}

void Camera::SetPosition(glm::vec3 position)
{
	glm::vec3 temp(GetView()[3].x, GetView()[3].y, GetView()[3].z);
	m_ViewTransform = glm::lookAt(position, temp, glm::vec3(0, 1, 0));
}

void Camera::UpdatePos(GLFWwindow* window)
{
	float currentTime = (float)glfwGetTime();
	float deltaTime = currentTime - m_PreviousTime;
	float moveDist = m_Speed * deltaTime;

	int W = glfwGetKey(window, GLFW_KEY_W);
	int A = glfwGetKey(window, GLFW_KEY_A);
	int S = glfwGetKey(window, GLFW_KEY_S);
	int D = glfwGetKey(window, GLFW_KEY_D);

	// Keys
	if (W == GLFW_PRESS)
	{
		printf("W");
		m_WorldTransform = glm::translate(GetWorldTransform(), glm::vec3(0, 0, -moveDist));
		m_ViewTransform = glm::inverse(m_WorldTransform);
	}
	else if (A == GLFW_PRESS)
	{
		printf("A");
		m_WorldTransform = glm::translate(GetWorldTransform(), glm::vec3(-moveDist, 0, 0));
		m_ViewTransform = glm::inverse(m_WorldTransform);
	}
	else if (S == GLFW_PRESS)
	{
		printf("S");
		m_WorldTransform = glm::translate(GetWorldTransform(), glm::vec3(0, 0, moveDist));
		m_ViewTransform = glm::inverse(m_WorldTransform);
	}
	else if (D == GLFW_PRESS)
	{
		printf("D");
		m_WorldTransform = glm::translate(GetWorldTransform(), glm::vec3(moveDist, 0, 0));
		m_ViewTransform = glm::inverse(m_WorldTransform);
	}

	// Mouse
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	currentMouse = glm::vec3(x, y, 0);

	glm::vec3 up = glm::vec3(m_WorldTransform[1][2], m_WorldTransform[2][2], m_WorldTransform[3][3]);

	if (glfwGetMouseButton(window, 1)) 
	{
		float result = 0.f;
		if (currentMouse.x < previousMouse.x) 
		{
			// moving left.
			result = previousMouse.x - currentMouse.x;
			m_WorldTransform *= glm::rotate((result * 0.5f) * deltaTime, glm::vec3(0, 1, 0));
			m_ViewTransform = glm::inverse(m_WorldTransform);
		}
		else if (currentMouse.x > previousMouse.x) 
		{
			// moving right.
			result = currentMouse.x - previousMouse.x;
			m_WorldTransform *= glm::rotate( (result * 0.5f) * deltaTime, glm::vec3(0, -1, 0));
			m_ViewTransform = glm::inverse(m_WorldTransform);
		}

		if (currentMouse.y < previousMouse.y) 
		{
			// moving up.
			result = previousMouse.y - currentMouse.y;
			m_WorldTransform *= glm::rotate( (result * 0.5f) * deltaTime, glm::vec3(1, 0, 0));
			m_ViewTransform = glm::inverse(m_WorldTransform);
		}
		else if (currentMouse.y > previousMouse.y) 
		{
			// moving down.
			result = currentMouse.y - previousMouse.y;
			m_WorldTransform *= glm::rotate( (result * 0.5f) * deltaTime, glm::vec3(-1, 0, 0));
			m_ViewTransform = glm::inverse(m_WorldTransform);
		}

		glm::mat4 oldTransform = m_WorldTransform;

		glm::mat4 newTransform;

		glm::vec3 worldUp = glm::vec3(0, 1, 0);

		//Right
		glm::vec3 oldForward = glm::vec3(oldTransform[2].x, oldTransform[2].y, oldTransform[2].z);
		newTransform[0] = glm::normalize(glm::vec4(glm::cross(worldUp, oldForward), 0));

		//Up
		glm::vec3 newRight = glm::vec3(newTransform[0].x, newTransform[0].y, newTransform[0].z);
		newTransform[1] = glm::normalize(glm::vec4(glm::cross(oldForward, newRight), 0));
		
		//Forward
		newTransform[2] = glm::normalize(oldTransform[2]);

		//Position
		newTransform[3] = oldTransform[3];

		m_WorldTransform = newTransform;
		m_ViewTransform = glm::inverse(m_WorldTransform);

		previousMouse = currentMouse;
	}
	else {
		previousMouse = currentMouse;
	}
	m_PreviousTime = currentTime;
}

mat4 Camera::GetWorldTransform() {
	m_WorldTransform = glm::inverse(m_ViewTransform);
	return m_WorldTransform;
}

mat4 Camera::GetView()		 { return m_ViewTransform; }

mat4 Camera::GetProjection() { return m_ProjectionTransform; }

mat4 Camera::GetProjectionView() {
	m_ProjectionViewTransform = m_ProjectionTransform * m_ViewTransform;
	return m_ProjectionViewTransform;
}