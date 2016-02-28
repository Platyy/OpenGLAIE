#include "Shader.h"

Shader::~Shader()
{
	delete render;
}

void Shader::ShaderStart(glm::mat4 projectionViewMatrix, unsigned int VAO)
{
	render = new Rendering();

	// Creating shaders
	vsSource = "#version 410\n \
	layout(location=0) in vec4 Position; \
	layout(location=1) in vec4 Colour; \
	out vec4 vColour; \
	uniform mat4 ProjectionView; \
	void main() { vColour = Colour; gl_Position = ProjectionView * Position; }";

	fsSource = "#version 410\n \
	in vec4 vColour; \
	out vec4 FragColor; \
	void main() { FragColor = vColour; }";

	// Compiling shaders
	success = GL_FALSE;
	m_VertexShader = glCreateShader(GL_VERTEX_SHADER);
	m_FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(m_VertexShader, 1, (const char**)&vsSource, 0);
	glCompileShader(m_VertexShader);
	glShaderSource(m_FragmentShader, 1, (const char**)&fsSource, 0);
	glCompileShader(m_FragmentShader);

	// Linking shaders to program, not doing anything if it's not possible.
	m_ProgramID = glCreateProgram();
	glAttachShader(m_ProgramID, m_VertexShader);
	glAttachShader(m_ProgramID, m_FragmentShader);
	glLinkProgram(m_ProgramID);
	glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetProgramiv(m_ProgramID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];

		glGetProgramInfoLog(m_ProgramID, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}
	glDeleteShader(m_FragmentShader);
	glDeleteShader(m_VertexShader);

	glUseProgram(m_ProgramID);
	unsigned int ProjectionViewUniform = glGetUniformLocation(m_ProgramID, "ProjectionView");
	glUniformMatrix4fv(ProjectionViewUniform, 1, false, glm::value_ptr(projectionViewMatrix));

	glBindVertexArray(VAO);
	indexCount = (45);
}
