#pragma once
#include "AntTweakBar.h"
#include "GLFW\glfw3.h"
#include "glm\glm.hpp"
class GUI
{
public:
	GUI();
	~GUI();

	TwBar* m_Bar;

	glm::vec4 m_ClearColour = glm::vec4(1, 1, 1, 1);

	glm::vec3 m_TerrainColour = glm::vec3(1, 1, 1);
	glm::vec3 m_TerrainPosition = glm::vec3(0, 10, 0);

	struct Position
	{
		float x, y, z;
	};

	Position pos;
	Position rot;

	bool m_RenderChar = false;

	float m_OffsetGrass;
	float m_OffsetRocks;

	void Init(GLFWwindow* _window);
	void Draw();
	void Close();

	static void OnMouseButton(GLFWwindow*, int b, int a, int m) {
		TwEventMouseButtonGLFW(b, a);
	}

	static void OnMousePosition(GLFWwindow*, double x, double y) {
		TwEventMousePosGLFW((int)x, (int)y);
	}

	static void OnMouseScroll(GLFWwindow*, double x, double y) {
		TwEventMouseWheelGLFW((int)y);
	}

	static void OnKey(GLFWwindow*, int k, int s, int a, int m) {
		TwEventKeyGLFW(k, a);
	}

	static void OnChar(GLFWwindow*, unsigned int c) {
		TwEventCharGLFW(c, GLFW_PRESS);
	}

	static void OnWindowResize(GLFWwindow*, int w, int h) {
		TwWindowSize(w, h);
		glViewport(0, 0, w, h);
	}
};

