#include "render/shader.h"
#include <iostream>
#include <fstream>
#include <sstream>


Shader::Shader()
{
}

Shader::~Shader()
{
}

void Shader::SetShadersFileName(const std::string &vFileName, const std::string &sFileName)
{
	vertexShaderFileName = vFileName;
	fragmentShaderFileName = sFileName;
}

void Shader::CheckShaderCompileStatus(GLuint shader)
{
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		GLint logLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
		GLchar* buffer = new GLchar[logLength];
		GLsizei bufferSize;
		glGetShaderInfoLog(shader, logLength, &bufferSize, buffer);
		std::cout << "unsuccessful" << std::endl;
		std::cout << buffer << std::endl;
		delete[] buffer;

		return;
	}
	else {
		std::cout << "successful" << std::endl;
	}
}

void Shader::Init()
{
	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

	std::string vstr = ReadShader(vertexShaderFileName);
	std::string fstr = ReadShader(fragmentShaderFileName);

	const char* vsText = vstr.c_str();
	glShaderSource(vertShader, 1, &vsText, 0);
	const char* fsText = fstr.c_str();
	glShaderSource(fragShader, 1, &fsText, 0);

	glCompileShader(vertShader);
	std::cout << "Vertex shader compilation ";
	CheckShaderCompileStatus(vertShader);

	glCompileShader(fragShader);
	std::cout << "Fragment shader compilation ";
	CheckShaderCompileStatus(fragShader);

	programID = glCreateProgram();
	glAttachShader(programID, vertShader);
	glAttachShader(programID, fragShader);

	glLinkProgram(programID);
	GLint status;
	glGetProgramiv(programID, GL_LINK_STATUS, &status);
	if (!status) {
		std::cerr << "Unable to link the shaders" << std::endl;
		return;
	}
}

std::string Shader::ReadShader(const std::string &name)
{
	GLint status;

	std::ifstream ifs;
	std::string str;
	std::stringstream ss;

	ifs.open(name);
	if (!ifs) {
		std::cerr << "Failed to open the shader file:" << name << std::endl;
		return 0;
	}
	ss << ifs.rdbuf();
	ifs.close();
	str = ss.str();

	return str;
}

void Shader::SendAttributeData(std::vector<float>& buffer, const char* name)
{
	GLuint bufferID;
	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * buffer.size(), &buffer[0], GL_STATIC_DRAW);
	GLint aLoc = glGetAttribLocation(programID, name);
	glEnableVertexAttribArray(aLoc);
	glVertexAttribPointer(aLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

// Send an integer to the shader.
void Shader::SendUniformData(int input, const char* name)
{

	glUniform1i(glGetUniformLocation(programID, name), input);
}

// Send a float number to the shader.
void Shader::SendUniformData(float input, const char* name)
{
	glUniform1f(glGetUniformLocation(programID, name), input);
}

// Send a vec3 to the shader.
void Shader::SendUniformData(glm::vec3 input, const char* name)
{
	glUniform3f(glGetUniformLocation(programID, name), input.x, input.y, input.z);
}

//send a matrix to the shader.
void Shader::SendUniformData(glm::mat4 &input, const char* name)
{
	glUniformMatrix4fv(glGetUniformLocation(programID, name), 1, GL_FALSE, &input[0][0]);
}

void Shader::Bind()
{
	glUseProgram(programID);
}

void Shader::Unbind()
{
	glUseProgram(0);
}
