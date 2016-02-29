#include "GUI.h"

GUI::GUI()
{
}

GUI::~GUI()
{
}

void GUI::Init(GLFWwindow* _window)
{
	TwInit(TW_OPENGL_CORE, nullptr);
	TwWindowSize(1280, 720);

	glfwSetMouseButtonCallback(_window, OnMouseButton);
	glfwSetCursorPosCallback(_window, OnMousePosition);
	glfwSetScrollCallback(_window, OnMouseScroll);
	glfwSetKeyCallback(_window, OnKey);
	glfwSetCharCallback(_window, OnChar);
	glfwSetWindowSizeCallback(_window, OnWindowResize);

	m_Bar = TwNewBar("UI");

	TwStructMember myStruct[] = {
		{"x", TW_TYPE_FLOAT, offsetof(Position, x), ""},
		{"y", TW_TYPE_FLOAT, offsetof(Position, y), "" },
		{"z", TW_TYPE_FLOAT, offsetof(Position, z), "" }
	};
	TwType myStructType;
	myStructType = TwDefineStruct("MySTRUCT", myStruct, 3, sizeof(Position), NULL, NULL);

	TwAddVarRW(m_Bar, "Clear Colour", TW_TYPE_COLOR4F, &m_ClearColour[0], "");
	
	TwAddVarRW(m_Bar, "Render", TW_TYPE_BOOLCPP, &m_RenderChar, "true=Enabled false=Disabled");

	TwAddVarRW(m_Bar, "Grass", TW_TYPE_FLOAT, &m_OffsetGrass, NULL);
	TwAddVarRW(m_Bar, "Rocks", TW_TYPE_FLOAT, &m_OffsetRocks, NULL);

	TwAddVarRW(m_Bar, "Position", myStructType, &pos, NULL);
	TwAddVarRW(m_Bar, "Rotation", myStructType, &rot, NULL);

	TwAddVarRW(m_Bar, "Terrain Light Colour", myStructType, &m_TerrainColour, NULL);
	TwAddVarRW(m_Bar, "Terrain Light Position", myStructType, &m_TerrainPosition, NULL);
}

void GUI::Draw()
{
	//TwDraw();
	glClearColor(m_ClearColour.r, m_ClearColour.g, m_ClearColour.b, m_ClearColour.a);
}

void GUI::Close()
{
	TwDeleteAllBars();
	TwTerminate();
}