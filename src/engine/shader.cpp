#include "shader.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

Shader::Shader(const char* vertexLocation, const char* fragmentLocation) {
	std::ifstream vertexFile;
	std::ifstream fragmentFile;

	vertexFile.open(vertexLocation);
	fragmentFile.open(fragmentLocation);

	std::stringstream vertexStream, fragmentStream;
	vertexStream << vertexFile.rdbuf();
	fragmentStream << fragmentFile.rdbuf();

	vertexFile.close();
	fragmentFile.close();

	std::string vertexCode = vertexStream.str();
	std::string fragmentCode = fragmentStream.str();

	const char* vertexConstCode = vertexCode.c_str();
	const char* fragmentConstCode = fragmentCode.c_str();

	GLuint vertexShader, fragmentShader;
	GLint success;
	GLchar infoLog[512];

	std::cout << "Compiling vertex shader...\n";
	
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexConstCode, nullptr);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		std::cout << "Failed to compile vertex shader:\n" << infoLog << '\n';
	}

	std::cout << "Compiling fragment shader...\n";
	
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentConstCode, nullptr);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "Failed to compile fragment shader:\n" << infoLog << '\n';
	}

	std::cout << "Compiling shader program...\n";
	
	this->id = glCreateProgram();
	glAttachShader(this->id, vertexShader);
	glAttachShader(this->id, fragmentShader);
	glLinkProgram(this->id);

	glGetProgramiv(this->id, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(this->id, 512, NULL, infoLog);
		std::cout << "Failed to compile shader program:\n" << infoLog << '\n';
	}
	
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}
Shader::~Shader() {
	this->destroy();
}

void Shader::use() const {
	glUseProgram(this->id);
}
void Shader::destroy() {
	glDeleteProgram(this->id);
	this->id = 0;
}

void Shader::drop() {
	glUseProgram(0);
}

void Shader::setInt(const char* name, int value) const {
	glUniform1i(glGetUniformLocation(this->id, name), value);
}
void Shader::setFloat(const char* name, float value) const {
	glUniform1f(glGetUniformLocation(this->id, name), value);
}
void Shader::setVector2(const char* name, glm::vec2 value) const {
	glUniform2f(glGetUniformLocation(this->id, name), value.x, value.y);
}
void Shader::setVector3(const char* name, glm::vec3 value) const {
	glUniform3f(glGetUniformLocation(this->id, name), value.x, value.y, value.z);
}
void Shader::setVector4(const char* name, glm::vec4 value) const {
	glUniform4f(glGetUniformLocation(this->id, name), value.x, value.y, value.z, value.z);
}
void Shader::setMatrix4(const char* name, glm::mat4 value) const {
	glUniformMatrix4fv(glGetUniformLocation(this->id, name), 1, false, &value[0][0]);
}