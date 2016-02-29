#include "Terrain.h"

Terrain::Terrain()
{
}

Terrain::~Terrain()
{
}

void Terrain::GenGrid(unsigned int _rows, unsigned int _cols, const char* terrainVS, const char* terrainFS)
{
	int success = GL_FALSE;
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vertexShader, 1, (const char**)&terrainVS, 0);
	glCompileShader(vertexShader);
	glShaderSource(fragmentShader, 1, (const char**)&terrainFS, 0);
	glCompileShader(fragmentShader);
	m_ProgramID = glCreateProgram();
	glAttachShader(m_ProgramID, vertexShader);
	glAttachShader(m_ProgramID, fragmentShader);
	glLinkProgram(m_ProgramID);
	glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
		int infoLogLength = 0;
		glGetProgramiv(m_ProgramID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(m_ProgramID, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}

	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	int dims = 64;
	int octaves = 3;
	float *perlinData = new float[dims * dims];
	float scale = (2.0f / dims) * 3;
	for (int x = 0; x < 64; ++x)
	{
		for (int y = 0; y < 64; ++y)
		{
			float amplitude = 1.f;
			float persistence = 0.3f;

			perlinData[y * dims + x] = 0;
			for (int o = 0; o < octaves; ++o)
			{
				float freq = powf(2, (float)o);
				float perlinSample = glm::perlin(glm::vec2((float)x, (float)y) * scale * freq) * 0.5f + 0.5f;
				perlinData[y * dims + x] += perlinSample * amplitude;
				amplitude *= persistence;
			}
		}
	}

	glGenTextures(1, &m_PerlinTexture);
	glBindTexture(GL_TEXTURE_2D, m_PerlinTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, 64, 64, 0, GL_RED, GL_FLOAT, perlinData);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	int imageFormat = 0, imageHeight = 0, imageWidth = 0;
	unsigned char* data;
	data = stbi_load("./data/Textures/grass.png", &imageWidth, &imageHeight, &imageFormat, STBI_default);
	glGenTextures(1, &m_GrassTexture);
	glBindTexture(GL_TEXTURE_2D, m_GrassTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //???
	stbi_image_free(data);

	data = stbi_load("./data/Textures/rock.png", &imageWidth, &imageHeight, &imageFormat, STBI_default);
	glGenTextures(1, &m_RockTexture);
	glBindTexture(GL_TEXTURE_2D, m_RockTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	stbi_image_free(data);

	Verts* aoVertices = new Verts[_rows * _cols];
	for (unsigned int r = 0; r < _rows; ++r)
	{
		for (unsigned int c = 0; c < _cols; ++c)
		{
			aoVertices[r * _cols + c].position = glm::vec4((float)c, 0, (float)r, 1);

			// calc tex coords 0-1
			aoVertices[r * _cols + c].texCoord = glm::vec2((float)r / _rows, (float)c / _cols);
		}
	}

	// Index array
	unsigned int* auiIndices = new unsigned int[(_rows - 1) * (_cols - 1) * 6];
	unsigned int index = 0;
	for (unsigned int r = 0; r < (_rows - 1); ++r)
	{
		for (unsigned int c = 0; c < (_cols - 1); ++c)
		{
			auiIndices[index++] = r * _cols + c;
			auiIndices[index++] = (r + 1) * _cols + c;
			auiIndices[index++] = (r + 1) * _cols + (c + 1);

			auiIndices[index++] = r * _cols + c;
			auiIndices[index++] = (r + 1) * _cols + (c + 1);
			auiIndices[index++] = r * _cols + (c + 1);
		}
	}

	indexCount = (_rows - 1) * (_cols - 1) * 6;

	glGenVertexArrays(1, &m_VAO);
	
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);

	glBindVertexArray(m_VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, (_rows * _cols) * sizeof(Verts), aoVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (_rows - 1) * (_cols - 1) * 6 * sizeof(unsigned int), auiIndices, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Verts), (void*)offsetof(Verts, position));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Verts), (void*)offsetof(Verts, texCoord));
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	delete[] aoVertices;
}

void Terrain::RenderGrid(glm::mat4 projView, float grass, float rock)
{
	glUseProgram(m_ProgramID);
	unsigned int loc = glGetUniformLocation(m_ProgramID, "ProjectionView");
	glUniformMatrix4fv(loc, 1, false, glm::value_ptr(projView));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_PerlinTexture);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_GrassTexture);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_RockTexture);

	loc = glGetUniformLocation(m_ProgramID, "grassOffset");
	glUniform1f(loc, grass * 0.1f);

	loc = glGetUniformLocation(m_ProgramID, "rocksOffset");
	glUniform1f(loc, rock * 0.1f);

	loc = glGetUniformLocation(m_ProgramID, "perlinMap");
	glUniform1i(loc, 0);

	loc = glGetUniformLocation(m_ProgramID, "grassDiffuse");
	glUniform1i(loc, 2);

	loc = glGetUniformLocation(m_ProgramID, "rocksDiffuse");
	glUniform1i(loc, 3);

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}